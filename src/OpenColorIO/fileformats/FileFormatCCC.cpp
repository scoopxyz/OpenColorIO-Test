// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenColorIO Project.

#include <map>

#include <OpenColorIO/OpenColorIO.h>

#include "fileformats/cdl/CDLParser.h"
#include "transforms/CDLTransform.h"
#include "transforms/FileTransform.h"
#include "OpBuilders.h"
#include "ParseUtils.h"
#include "pystring/pystring.h"

OCIO_NAMESPACE_ENTER
{
    ////////////////////////////////////////////////////////////////
    
    namespace
    {
        class LocalCachedFile : public CachedFile
        {
        public:
            LocalCachedFile () {};
            ~LocalCachedFile() {};
            
            CDLTransformMap transformMap;
            CDLTransformVec transformVec;
        };
        
        typedef OCIO_SHARED_PTR<LocalCachedFile> LocalCachedFileRcPtr;
        
        class LocalFileFormat : public FileFormat
        {
        public:
            
            ~LocalFileFormat() {};
            
            void getFormatInfo(FormatInfoVec & formatInfoVec) const override;
            
            CachedFileRcPtr read(
                std::istream & istream,
                const std::string & fileName) const override;
            
            void buildFileOps(OpRcPtrVec & ops,
                              const Config& config,
                              const ConstContextRcPtr & context,
                              CachedFileRcPtr untypedCachedFile,
                              const FileTransform& fileTransform,
                              TransformDirection dir) const override;
        };
        
        void LocalFileFormat::getFormatInfo(FormatInfoVec & formatInfoVec) const
        {
            FormatInfo info;
            info.name = "ColorCorrectionCollection";
            info.extension = "ccc";
            info.capabilities = FORMAT_CAPABILITY_READ;
            formatInfoVec.push_back(info);
        }
        
        // Try and load the format
        // Raise an exception if it can't be loaded.
        
        CachedFileRcPtr LocalFileFormat::read(
            std::istream & istream,
            const std::string & fileName) const
        {
            CDLParser parser(fileName);
            parser.parse(istream);

            LocalCachedFileRcPtr cachedFile = LocalCachedFileRcPtr(new LocalCachedFile());
            
            parser.getCDLTransforms(cachedFile->transformMap,
                                    cachedFile->transformVec);

            return cachedFile;
        }
        
        void
        LocalFileFormat::buildFileOps(OpRcPtrVec & ops,
                                      const Config& config,
                                      const ConstContextRcPtr & context,
                                      CachedFileRcPtr untypedCachedFile,
                                      const FileTransform& fileTransform,
                                      TransformDirection dir) const
        {
            LocalCachedFileRcPtr cachedFile = DynamicPtrCast<LocalCachedFile>(untypedCachedFile);
            
            // This should never happen.
            if(!cachedFile)
            {
                std::ostringstream os;
                os << "Cannot build .ccc Op. Invalid cache type.";
                throw Exception(os.str().c_str());
            }
            
            TransformDirection newDir = CombineTransformDirections(dir,
                fileTransform.getDirection());
            if(newDir == TRANSFORM_DIR_UNKNOWN)
            {
                std::ostringstream os;
                os << "Cannot build ASC FileTransform,";
                os << " unspecified transform direction.";
                throw Exception(os.str().c_str());
            }
            
            // Below this point, we should throw ExceptionMissingFile on
            // errors rather than Exception
            // This is because we've verified that the ccc file is valid,
            // at now we're only querying whether the specified cccid can
            // be found.
            //
            // Using ExceptionMissingFile enables the missing looks fallback
            // mechanism to function properly.
            // At the time ExceptionMissingFile was named, we errently assumed
            // a 1:1 relationship between files and color corrections, which is
            // not true for .ccc files.
            //
            // In a future OCIO release, it may be more appropriate to
            // rename ExceptionMissingFile -> ExceptionMissingCorrection.
            // But either way, it's what we should throw below.
            
            std::string cccid = fileTransform.getCCCId();
            cccid = context->resolveStringVar(cccid.c_str());
            
            if(cccid.empty())
            {
                std::ostringstream os;
                os << "You must specify which cccid to load from the ccc file";
                os << " (either by name or index).";
                throw ExceptionMissingFile(os.str().c_str());
            }
            
            bool success=false;
            
            // Try to parse the cccid as a string id
            CDLTransformMap::const_iterator iter = cachedFile->transformMap.find(cccid);
            if(iter != cachedFile->transformMap.end())
            {
                success = true;
                BuildCDLOps(ops,
                            config,
                            *(iter->second),
                            newDir);
            }
            
            // Try to parse the cccid as an integer index
            // We want to be strict, so fail if leftover chars in the parse.
            if(!success)
            {
                int cccindex=0;
                if(StringToInt(&cccindex, cccid.c_str(), true))
                {
                    int maxindex = ((int)cachedFile->transformVec.size())-1;
                    if(cccindex<0 || cccindex>maxindex)
                    {
                        std::ostringstream os;
                        os << "The specified cccindex " << cccindex;
                        os << " is outside the valid range for this file [0,";
                        os << maxindex << "]";
                        throw ExceptionMissingFile(os.str().c_str());
                    }
                    
                    success = true;
                    BuildCDLOps(ops,
                                config,
                                *cachedFile->transformVec[cccindex],
                                newDir);
                }
            }
            
            if(!success)
            {
                std::ostringstream os;
                os << "You must specify a valid cccid to load from the ccc file";
                os << " (either by name or index). id='" << cccid << "' ";
                os << "is not found in the file, and is not parsable as an ";
                os << "integer index.";
                throw ExceptionMissingFile(os.str().c_str());
            }
        }
    }
    
    FileFormat * CreateFileFormatCCC()
    {
        return new LocalFileFormat();
    }
}
OCIO_NAMESPACE_EXIT

#ifdef OCIO_UNIT_TEST

namespace OCIO = OCIO_NAMESPACE;

#include "UnitTest.h"
#include "UnitTestUtils.h"

OCIO::LocalCachedFileRcPtr LoadCCCFile(const std::string & fileName)
{
    return OCIO::LoadTestFile<OCIO::LocalFileFormat, OCIO::LocalCachedFile>(
        fileName, std::ios_base::in);
}

OCIO_ADD_TEST(FileFormatCCC, TestCCC)
{
    // CCC file
    const std::string fileName("cdl_test1.ccc");

    OCIO::LocalCachedFileRcPtr cccFile;
    OCIO_CHECK_NO_THROW(cccFile = LoadCCCFile(fileName));
    
    OCIO_CHECK_EQUAL(5, cccFile->transformVec.size());
    // Two of the five CDLs in the file don't have an id attribute and are not
    // included in the transformMap since it used the id as the key.
    OCIO_CHECK_EQUAL(3, cccFile->transformMap.size());
    {
        std::string idStr(cccFile->transformVec[0]->getID());
        OCIO_CHECK_EQUAL("cc0001", idStr);
        std::string descStr(cccFile->transformVec[0]->getDescription());
        // OCIO keeps only the first SOPNode description
        OCIO_CHECK_EQUAL("Example look", descStr);
        float slope[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[0]->getSlope(slope));
        OCIO_CHECK_EQUAL(1.0f, slope[0]);
        OCIO_CHECK_EQUAL(1.0f, slope[1]);
        OCIO_CHECK_EQUAL(0.9f, slope[2]);
        float offset[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[0]->getOffset(offset));
        OCIO_CHECK_EQUAL(-0.03f, offset[0]);
        OCIO_CHECK_EQUAL(-0.02f, offset[1]);
        OCIO_CHECK_EQUAL(0.0f, offset[2]);
        float power[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[0]->getPower(power));
        OCIO_CHECK_EQUAL(1.25f, power[0]);
        OCIO_CHECK_EQUAL(1.0f, power[1]);
        OCIO_CHECK_EQUAL(1.0f, power[2]);
        OCIO_CHECK_EQUAL(1.7, cccFile->transformVec[0]->getSat());
    }
    {
        std::string idStr(cccFile->transformVec[1]->getID());
        OCIO_CHECK_EQUAL("cc0002", idStr);
        std::string descStr(cccFile->transformVec[1]->getDescription());
        OCIO_CHECK_EQUAL("pastel", descStr);
        float slope[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[1]->getSlope(slope));
        OCIO_CHECK_EQUAL(0.9f, slope[0]);
        OCIO_CHECK_EQUAL(0.7f, slope[1]);
        OCIO_CHECK_EQUAL(0.6f, slope[2]);
        float offset[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[1]->getOffset(offset));
        OCIO_CHECK_EQUAL(0.1f, offset[0]);
        OCIO_CHECK_EQUAL(0.1f, offset[1]);
        OCIO_CHECK_EQUAL(0.1f, offset[2]);
        float power[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[1]->getPower(power));
        OCIO_CHECK_EQUAL(0.9f, power[0]);
        OCIO_CHECK_EQUAL(0.9f, power[1]);
        OCIO_CHECK_EQUAL(0.9f, power[2]);
        OCIO_CHECK_EQUAL(0.7, cccFile->transformVec[1]->getSat());
    }
    {
        std::string idStr(cccFile->transformVec[2]->getID());
        OCIO_CHECK_EQUAL("cc0003", idStr);
        std::string descStr(cccFile->transformVec[2]->getDescription());
        OCIO_CHECK_EQUAL("golden", descStr);
        float slope[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[2]->getSlope(slope));
        OCIO_CHECK_EQUAL(1.2f, slope[0]);
        OCIO_CHECK_EQUAL(1.1f, slope[1]);
        OCIO_CHECK_EQUAL(1.0f, slope[2]);
        float offset[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[2]->getOffset(offset));
        OCIO_CHECK_EQUAL(0.0f, offset[0]);
        OCIO_CHECK_EQUAL(0.0f, offset[1]);
        OCIO_CHECK_EQUAL(0.0f, offset[2]);
        float power[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[2]->getPower(power));
        OCIO_CHECK_EQUAL(0.9f, power[0]);
        OCIO_CHECK_EQUAL(1.0f, power[1]);
        OCIO_CHECK_EQUAL(1.2f, power[2]);
        OCIO_CHECK_EQUAL(1.0, cccFile->transformVec[2]->getSat());
    }
    {
        std::string idStr(cccFile->transformVec[3]->getID());
        OCIO_CHECK_EQUAL("", idStr);
        std::string descStr(cccFile->transformVec[3]->getDescription());
        OCIO_CHECK_EQUAL("", descStr);
        float slope[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[3]->getSlope(slope));
        OCIO_CHECK_EQUAL(4.0f, slope[0]);
        OCIO_CHECK_EQUAL(5.0f, slope[1]);
        OCIO_CHECK_EQUAL(6.0f, slope[2]);
        float offset[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[3]->getOffset(offset));
        OCIO_CHECK_EQUAL(0.0f, offset[0]);
        OCIO_CHECK_EQUAL(0.0f, offset[1]);
        OCIO_CHECK_EQUAL(0.0f, offset[2]);
        float power[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[3]->getPower(power));
        OCIO_CHECK_EQUAL(0.9f, power[0]);
        OCIO_CHECK_EQUAL(1.0f, power[1]);
        OCIO_CHECK_EQUAL(1.2f, power[2]);
        // SatNode missing from XML, uses a default of 1.0.
        OCIO_CHECK_EQUAL(1.0, cccFile->transformVec[3]->getSat());
    }
    {
        std::string idStr(cccFile->transformVec[4]->getID());
        OCIO_CHECK_EQUAL("", idStr);
        // SOPNode missing from XML, uses default values.
        std::string descStr(cccFile->transformVec[4]->getDescription());
        OCIO_CHECK_EQUAL("", descStr);
        float slope[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[4]->getSlope(slope));
        OCIO_CHECK_EQUAL(1.0f, slope[0]);
        OCIO_CHECK_EQUAL(1.0f, slope[1]);
        OCIO_CHECK_EQUAL(1.0f, slope[2]);
        float offset[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[4]->getOffset(offset));
        OCIO_CHECK_EQUAL(0.0f, offset[0]);
        OCIO_CHECK_EQUAL(0.0f, offset[1]);
        OCIO_CHECK_EQUAL(0.0f, offset[2]);
        float power[3] = { 0.f, 0.f, 0.f };
        OCIO_CHECK_NO_THROW(cccFile->transformVec[4]->getPower(power));
        OCIO_CHECK_EQUAL(1.0f, power[0]);
        OCIO_CHECK_EQUAL(1.0f, power[1]);
        OCIO_CHECK_EQUAL(1.0f, power[2]);

        OCIO_CHECK_EQUAL(0.0, cccFile->transformVec[4]->getSat());
    }

}


#endif