// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenColorIO Project.

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iterator>

#include <OpenColorIO/OpenColorIO.h>

#include "ops/Lut1D/Lut1DOp.h"
#include "ops/Lut3D/Lut3DOp.h"
#include "ParseUtils.h"
#include "pystring/pystring.h"
#include "transforms/FileTransform.h"

/*

http://doc.iridas.com/index.php/LUT_Formats

#comments start with '#'
#title is currently ignored, but it's not an error to enter one
TITLE "title"

#LUT_1D_SIZE M or
#LUT_3D_SIZE M
#where M is the size of the texture
#a 3D texture has the size M x M x M
#e.g. LUT_3D_SIZE 16 creates a 16 x 16 x 16 3D texture
LUT_3D_SIZE 2

#Default input value range (domain) is 0.0 (black) to 1.0 (white)
#Specify other min/max values to map the cube to any custom input
#range you wish to use, for example if you're working with HDR data
DOMAIN_MIN 0.0 0.0 0.0
DOMAIN_MAX 1.0 1.0 1.0

#for 1D textures, the data is simply a list of floating point values,
#three per line, in RGB order
#for 3D textures, the data is also RGB, and ordered in such a way
#that the red coordinate changes fastest, then the green coordinate,
#and finally, the blue coordinate changes slowest:
0.0 0.0 0.0
1.0 0.0 0.0
0.0 1.0 0.0
1.0 1.0 0.0
0.0 0.0 1.0
1.0 0.0 1.0
0.0 1.0 1.0
1.0 1.0 1.0

#Note that the LUT data is not limited to any particular range
#and can contain values under 0.0 and over 1.0
#The processing application might however still clip the
#output values to the 0.0 - 1.0 range, depending on the internal
#precision of that application's pipeline
#IRIDAS applications generally use a floating point pipeline
#with little or no clipping


*/


OCIO_NAMESPACE_ENTER
{
    namespace
    {
        class LocalCachedFile : public CachedFile
        {
        public:
            LocalCachedFile () :
                has1D(false),
                has3D(false)
            {
                lut1D = Lut1D::Create();
                lut3D = Lut3D::Create();
            };
            ~LocalCachedFile() {};

            bool has1D;
            bool has3D;
            // TODO: Switch to the OpData classes.
            Lut1DRcPtr lut1D;
            Lut3DRcPtr lut3D;
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
            
            void bake(const Baker & baker,
                      const std::string & formatName,
                      std::ostream & ostream) const override;
            
            void buildFileOps(OpRcPtrVec & ops,
                              const Config& config,
                              const ConstContextRcPtr & context,
                              CachedFileRcPtr untypedCachedFile,
                              const FileTransform& fileTransform,
                              TransformDirection dir) const override;
        private:
            static void ThrowErrorMessage(const std::string & error,
                const std::string & fileName,
                int line,
                const std::string & lineContent);
        };

        void LocalFileFormat::ThrowErrorMessage(const std::string & error,
            const std::string & fileName,
            int line,
            const std::string & lineContent)
        {
            std::ostringstream os;
            os << "Error parsing Iridas .cube file (";
            os << fileName;
            os << ").  ";
            if (-1 != line)
            {
                os << "At line (" << line << "): '";
                os << lineContent << "'.  ";
            }
            os << error;

            throw Exception(os.str().c_str());
        }

        void LocalFileFormat::getFormatInfo(FormatInfoVec & formatInfoVec) const
        {
            FormatInfo info;
            info.name = "iridas_cube";
            info.extension = "cube";
            info.capabilities = FORMAT_CAPABILITY_READ | FORMAT_CAPABILITY_BAKE;
            formatInfoVec.push_back(info);
        }

        CachedFileRcPtr
        LocalFileFormat::read(
            std::istream & istream,
            const std::string & fileName) const
        {
            // this shouldn't happen
            if(!istream)
            {
                throw Exception ("File stream empty when trying to read Iridas .cube LUT");
            }

            // Parse the file
            std::vector<float> raw;

            int size3d[] = { 0, 0, 0 };
            int size1d = 0;

            bool in1d = false;
            bool in3d = false;

            float domain_min[] = { 0.0f, 0.0f, 0.0f };
            float domain_max[] = { 1.0f, 1.0f, 1.0f };

            {
                std::string line;
                StringVec parts;
                std::vector<float> tmpfloats;
                int lineNumber = 0;

                while(nextline(istream, line))
                {
                    ++lineNumber;
                    // All lines starting with '#' are comments
                    if(pystring::startswith(line,"#")) continue;

                    // Strip, lowercase, and split the line
                    pystring::split(pystring::lower(pystring::strip(line)), parts);
                    if(parts.empty()) continue;

                    if(pystring::lower(parts[0]) == "title")
                    {
                        // Optional, and currently unhandled
                    }
                    else if(pystring::lower(parts[0]) == "lut_1d_size")
                    {
                        if(parts.size() != 2
                            || !StringToInt( &size1d, parts[1].c_str()))
                        {
                            ThrowErrorMessage(
                                "Malformed LUT_1D_SIZE tag.",
                                fileName,
                                lineNumber,
                                line);
                        }

                        raw.reserve(3*size1d);
                        in1d = true;
                    }
                    else if(pystring::lower(parts[0]) == "lut_2d_size")
                    {
                        ThrowErrorMessage(
                            "Unsupported tag: 'LUT_2D_SIZE'.",
                            fileName,
                            lineNumber,
                            line);
                    }
                    else if(pystring::lower(parts[0]) == "lut_3d_size")
                    {
                        int size = 0;

                        if(parts.size() != 2
                            || !StringToInt( &size, parts[1].c_str()))
                        {
                            ThrowErrorMessage(
                                "Malformed LUT_3D_SIZE tag.",
                                fileName,
                                lineNumber,
                                line);
                        }
                        size3d[0] = size;
                        size3d[1] = size;
                        size3d[2] = size;

                        raw.reserve(3*size3d[0]*size3d[1]*size3d[2]);
                        in3d = true;
                    }
                    else if(pystring::lower(parts[0]) == "domain_min")
                    {
                        if(parts.size() != 4 ||
                            !StringToFloat( &domain_min[0], parts[1].c_str()) ||
                            !StringToFloat( &domain_min[1], parts[2].c_str()) ||
                            !StringToFloat( &domain_min[2], parts[3].c_str()))
                        {
                            ThrowErrorMessage(
                                "Malformed DOMAIN_MIN tag.",
                                fileName,
                                lineNumber,
                                line);
                        }
                    }
                    else if(pystring::lower(parts[0]) == "domain_max")
                    {
                        if(parts.size() != 4 ||
                            !StringToFloat( &domain_max[0], parts[1].c_str()) ||
                            !StringToFloat( &domain_max[1], parts[2].c_str()) ||
                            !StringToFloat( &domain_max[2], parts[3].c_str()))
                        {
                            ThrowErrorMessage(
                                "Malformed DOMAIN_MAX tag.",
                                fileName,
                                lineNumber,
                                line);
                        }
                    }
                    else
                    {
                        // It must be a float triple!

                        if(!StringVecToFloatVec(tmpfloats, parts) || tmpfloats.size() != 3)
                        {
                            ThrowErrorMessage(
                                "Malformed color triples specified.",
                                fileName,
                                lineNumber,
                                line);
                        }

                        for(int i=0; i<3; ++i)
                        {
                            raw.push_back(tmpfloats[i]);
                        }
                    }
                }
            }

            // Interpret the parsed data, validate LUT sizes

            LocalCachedFileRcPtr cachedFile = LocalCachedFileRcPtr(new LocalCachedFile());

            if(in1d)
            {
                if(size1d != static_cast<int>(raw.size()/3))
                {
                    std::ostringstream os;
                    os << "Incorrect number of lut1d entries. ";
                    os << "Found " << raw.size() / 3;
                    os << ", expected " << size1d << ".";
                    ThrowErrorMessage(
                        os.str().c_str(),
                        fileName, -1, "");
                }

                // Reformat 1D data
                if(size1d>0)
                {
                    cachedFile->has1D = true;
                    memcpy(cachedFile->lut1D->from_min, domain_min, 3*sizeof(float));
                    memcpy(cachedFile->lut1D->from_max, domain_max, 3*sizeof(float));

                    for(int channel=0; channel<3; ++channel)
                    {
                        cachedFile->lut1D->luts[channel].resize(size1d);
                        for(int i=0; i<size1d; ++i)
                        {
                            cachedFile->lut1D->luts[channel][i] = raw[3*i+channel];
                        }
                    }

                    // 1e-5 rel error is a good threshold when float numbers near 0
                    // are written out with 6 decimal places of precision.  This is
                    // a bit aggressive, I.e., changes in the 6th decimal place will
                    // be considered roundoff error, but changes in the 5th decimal
                    // will be considered LUT 'intent'.
                    // 1.0
                    // 1.000005 equal to 1.0
                    // 1.000007 equal to 1.0
                    // 1.000010 not equal
                    // 0.0
                    // 0.000001 not equal

                    cachedFile->lut1D->maxerror = 1e-5f;
                    cachedFile->lut1D->errortype = Lut1D::ERROR_RELATIVE;
                }
            }
            else if(in3d)
            {
                cachedFile->has3D = true;

                if(size3d[0]*size3d[1]*size3d[2]
                    != static_cast<int>(raw.size()/3))
                {
                    std::ostringstream os;
                    os << "Incorrect number of 3D LUT entries. ";
                    os << "Found " << raw.size() / 3 << ", expected ";
                    os << size3d[0] * size3d[1] * size3d[2] << ".";
                    ThrowErrorMessage(
                        os.str().c_str(),
                        fileName, -1, "");
                }

                // Reformat 3D data
                memcpy(cachedFile->lut3D->from_min, domain_min, 3*sizeof(float));
                memcpy(cachedFile->lut3D->from_max, domain_max, 3*sizeof(float));
                cachedFile->lut3D->size[0] = size3d[0];
                cachedFile->lut3D->size[1] = size3d[1];
                cachedFile->lut3D->size[2] = size3d[2];
                cachedFile->lut3D->lut = raw;
            }
            else
            {
                ThrowErrorMessage(
                    "LUT type (1D/3D) unspecified.",
                    fileName, -1, "");
            }

            return cachedFile;
        }
        
        void LocalFileFormat::bake(const Baker & baker,
                                   const std::string & formatName,
                                   std::ostream & ostream) const
        {

            static const int DEFAULT_CUBE_SIZE = 32;

            if(formatName != "iridas_cube")
            {
                std::ostringstream os;
                os << "Unknown cube format name, '";
                os << formatName << "'.";
                throw Exception(os.str().c_str());
            }

            ConstConfigRcPtr config = baker.getConfig();

            int cubeSize = baker.getCubeSize();
            if(cubeSize==-1) cubeSize = DEFAULT_CUBE_SIZE;
            cubeSize = std::max(2, cubeSize); // smallest cube is 2x2x2

            std::vector<float> cubeData;
            cubeData.resize(cubeSize*cubeSize*cubeSize*3);
            GenerateIdentityLut3D(&cubeData[0], cubeSize, 3, LUT3DORDER_FAST_RED);
            PackedImageDesc cubeImg(&cubeData[0], cubeSize*cubeSize*cubeSize, 1, 3);

            // Apply our conversion from the input space to the output space.
            ConstProcessorRcPtr inputToTarget;
            std::string looks = baker.getLooks();
            if(!looks.empty())
            {
                LookTransformRcPtr transform = LookTransform::Create();
                transform->setLooks(looks.c_str());
                transform->setSrc(baker.getInputSpace());
                transform->setDst(baker.getTargetSpace());
                inputToTarget = config->getProcessor(transform, TRANSFORM_DIR_FORWARD);
            }
            else
            {
                inputToTarget = config->getProcessor(baker.getInputSpace(), baker.getTargetSpace());
            }
            ConstCPUProcessorRcPtr cpu = inputToTarget->getDefaultCPUProcessor();
            cpu->apply(cubeImg);

            if(baker.getMetadata() != NULL)
            {
                std::string metadata = baker.getMetadata();
                StringVec metadatavec;
                pystring::split(pystring::strip(metadata), metadatavec, "\n");
                if(metadatavec.size() > 0)
                {
                    for(size_t i = 0; i < metadatavec.size(); ++i)
                    {
                        ostream << "# " << metadatavec[i] << "\n";
                    }
                    ostream << "\n";
                }
            }
            ostream << "LUT_3D_SIZE " << cubeSize << "\n";
            if(cubeSize < 2)
            {
                throw Exception("Internal cube size exception");
            }

            // Set to a fixed 6 decimal precision
            ostream.setf(std::ios::fixed, std::ios::floatfield);
            ostream.precision(6);
            for(int i=0; i<cubeSize*cubeSize*cubeSize; ++i)
            {
                ostream << cubeData[3*i+0] << " "
                        << cubeData[3*i+1] << " "
                        << cubeData[3*i+2] << "\n";
            }
        }

        void
        LocalFileFormat::buildFileOps(OpRcPtrVec & ops,
                                      const Config& /*config*/,
                                      const ConstContextRcPtr & /*context*/,
                                      CachedFileRcPtr untypedCachedFile,
                                      const FileTransform& fileTransform,
                                      TransformDirection dir) const
        {
            LocalCachedFileRcPtr cachedFile = DynamicPtrCast<LocalCachedFile>(untypedCachedFile);

            // This should never happen.
            if(!cachedFile)
            {
                std::ostringstream os;
                os << "Cannot build Iridas .cube Op. Invalid cache type.";
                throw Exception(os.str().c_str());
            }

            TransformDirection newDir = CombineTransformDirections(dir,
                fileTransform.getDirection());
            if(newDir == TRANSFORM_DIR_UNKNOWN)
            {
                std::ostringstream os;
                os << "Cannot build file format transform,";
                os << " unspecified transform direction.";
                throw Exception(os.str().c_str());
            }

            // TODO: INTERP_LINEAR should not be hard-coded.
            // Instead query 'highest' interpolation?
            // (right now, it's linear). If cubic is added, consider
            // using it

            if(newDir == TRANSFORM_DIR_FORWARD)
            {
                if(cachedFile->has1D)
                {
                    CreateLut1DOp(ops, cachedFile->lut1D,
                                  INTERP_LINEAR, newDir);
                }
                if(cachedFile->has3D)
                {
                    CreateLut3DOp(ops, cachedFile->lut3D,
                                  fileTransform.getInterpolation(), newDir);
                }
            }
            else if(newDir == TRANSFORM_DIR_INVERSE)
            {
                if(cachedFile->has3D)
                {
                    CreateLut3DOp(ops, cachedFile->lut3D,
                                  fileTransform.getInterpolation(), newDir);
                }
                if(cachedFile->has1D)
                {
                    CreateLut1DOp(ops, cachedFile->lut1D,
                                  INTERP_LINEAR, newDir);
                }
            }
        }
    }

    FileFormat * CreateFileFormatIridasCube()
    {
        return new LocalFileFormat();
    }
}
OCIO_NAMESPACE_EXIT


///////////////////////////////////////////////////////////////////////////////

#ifdef OCIO_UNIT_TEST

#include <fstream>

#include "UnitTest.h"

namespace OCIO = OCIO_NAMESPACE;


OCIO_ADD_TEST(FileFormatIridasCube, FormatInfo)
{
    OCIO::FormatInfoVec formatInfoVec;
    OCIO::LocalFileFormat tester;
    tester.getFormatInfo(formatInfoVec);

    OCIO_CHECK_EQUAL(1, formatInfoVec.size());
    OCIO_CHECK_EQUAL("iridas_cube", formatInfoVec[0].name);
    OCIO_CHECK_EQUAL("cube", formatInfoVec[0].extension);
    OCIO_CHECK_EQUAL(OCIO::FORMAT_CAPABILITY_READ | OCIO::FORMAT_CAPABILITY_BAKE,
                     formatInfoVec[0].capabilities);
}

OCIO::LocalCachedFileRcPtr ReadIridasCube(const std::string & fileContent)
{
    std::istringstream is;
    is.str(fileContent);

    // Read file
    OCIO::LocalFileFormat tester;
    const std::string SAMPLE_NAME("Memory File");
    OCIO::CachedFileRcPtr cachedFile = tester.read(is, SAMPLE_NAME);

    return OCIO::DynamicPtrCast<OCIO::LocalCachedFile>(cachedFile);
}

OCIO_ADD_TEST(FileFormatIridasCube, ReadFailure)
{
    {
        // Validate stream can be read with no error.
        // Then stream will be altered to introduce errors.
        const std::string SAMPLE_NO_ERROR =
            "LUT_3D_SIZE 2\n"
            "DOMAIN_MIN 0.0 0.0 0.0\n"
            "DOMAIN_MAX 1.0 1.0 1.0\n"

            "0.0 0.0 0.0\n"
            "1.0 0.0 0.0\n"
            "0.0 1.0 0.0\n"
            "1.0 1.0 0.0\n"
            "0.0 0.0 1.0\n"
            "1.0 0.0 1.0\n"
            "0.0 1.0 1.0\n"
            "1.0 1.0 1.0\n";

        OCIO_CHECK_NO_THROW(ReadIridasCube(SAMPLE_NO_ERROR));
    }
    {
        // Wrong LUT_3D_SIZE tag
        const std::string SAMPLE_ERROR =
            "LUT_3D_SIZE 2 2\n"
            "DOMAIN_MIN 0.0 0.0 0.0\n"
            "DOMAIN_MAX 1.0 1.0 1.0\n"

            "0.0 0.0 0.0\n"
            "1.0 0.0 0.0\n"
            "0.0 1.0 0.0\n"
            "1.0 1.0 0.0\n"
            "0.0 0.0 1.0\n"
            "1.0 0.0 1.0\n"
            "0.0 1.0 1.0\n"
            "1.0 1.0 1.0\n";

        OCIO_CHECK_THROW_WHAT(ReadIridasCube(SAMPLE_ERROR),
                              OCIO::Exception,
                              "Malformed LUT_3D_SIZE tag");
    }
    {
        // Wrong DOMAIN_MIN tag
        const std::string SAMPLE_ERROR =
            "LUT_3D_SIZE 2\n"
            "DOMAIN_MIN 0.0 0.0\n"
            "DOMAIN_MAX 1.0 1.0 1.0\n"

            "0.0 0.0 0.0\n"
            "1.0 0.0 0.0\n"
            "0.0 1.0 0.0\n"
            "1.0 1.0 0.0\n"
            "0.0 0.0 1.0\n"
            "1.0 0.0 1.0\n"
            "0.0 1.0 1.0\n"
            "1.0 1.0 1.0\n";

        OCIO_CHECK_THROW_WHAT(ReadIridasCube(SAMPLE_ERROR),
                              OCIO::Exception,
                              "Malformed DOMAIN_MIN tag");
    }
    {
        // Wrong DOMAIN_MAX tag
        const std::string SAMPLE_ERROR =
            "LUT_3D_SIZE 2\n"
            "DOMAIN_MIN 0.0 0.0 0.0\n"
            "DOMAIN_MAX 1.0 1.0 1.0 1.0\n"

            "0.0 0.0 0.0\n"
            "1.0 0.0 0.0\n"
            "0.0 1.0 0.0\n"
            "1.0 1.0 0.0\n"
            "0.0 0.0 1.0\n"
            "1.0 0.0 1.0\n"
            "0.0 1.0 1.0\n"
            "1.0 1.0 1.0\n";

        OCIO_CHECK_THROW_WHAT(ReadIridasCube(SAMPLE_ERROR),
                              OCIO::Exception,
                              "Malformed DOMAIN_MAX tag");
    }
    {
        // Unexpected tag
        const std::string SAMPLE_ERROR =
            "LUT_3D_SIZE 2\n"
            "DOMAIN_MIN 0.0 0.0 0.0\n"
            "DOMAIN_MAX 1.0 1.0 1.0\n"
            "WRONG_TAG\n"
            "0.0 0.0 0.0\n"
            "1.0 0.0 0.0\n"
            "0.0 1.0 0.0\n"
            "1.0 1.0 0.0\n"
            "0.0 0.0 1.0\n"
            "1.0 0.0 1.0\n"
            "0.0 1.0 1.0\n"
            "1.0 1.0 1.0\n";

        OCIO_CHECK_THROW_WHAT(ReadIridasCube(SAMPLE_ERROR),
                              OCIO::Exception,
                              "Malformed color triples specified");
    }
    {
        // Wrong number of entries
        const std::string SAMPLE_ERROR =
            "LUT_3D_SIZE 2\n"
            "DOMAIN_MIN 0.0 0.0 0.0\n"
            "DOMAIN_MAX 1.0 1.0 1.0\n"

            "0.0 0.0 0.0\n"
            "1.0 0.0 0.0\n"
            "0.0 1.0 0.0\n"
            "1.0 1.0 0.0\n"
            "0.0 0.0 1.0\n"
            "1.0 0.0 1.0\n"
            "0.0 1.0 1.0\n"
            "0.0 1.0 1.0\n"
            "0.0 1.0 1.0\n"
            "1.0 1.0 1.0\n";

        OCIO_CHECK_THROW_WHAT(ReadIridasCube(SAMPLE_ERROR),
                              OCIO::Exception,
                              "Incorrect number of 3D LUT entries");
    }
}

OCIO_ADD_TEST(FileFormatIridasCube, no_shaper)
{
    // check baker output
    OCIO::ConfigRcPtr config = OCIO::Config::Create();
    {
        OCIO::ColorSpaceRcPtr cs = OCIO::ColorSpace::Create();
        cs->setName("lnf");
        cs->setFamily("lnf");
        config->addColorSpace(cs);
        config->setRole(OCIO::ROLE_REFERENCE, cs->getName());
    }
    {
        OCIO::ColorSpaceRcPtr cs = OCIO::ColorSpace::Create();
        cs->setName("target");
        cs->setFamily("target");
        config->addColorSpace(cs);
    }

    std::ostringstream bout;
    bout << "# Alexa conversion LUT, logc2video. Full in/full out." << "\n";
    bout << "# created by alexalutconv (2.11)"                      << "\n";
    bout << ""                                                      << "\n";
    bout << "LUT_3D_SIZE 2"                                         << "\n";
    bout << "0.000000 0.000000 0.000000"                            << "\n";
    bout << "1.000000 0.000000 0.000000"                            << "\n";
    bout << "0.000000 1.000000 0.000000"                            << "\n";
    bout << "1.000000 1.000000 0.000000"                            << "\n";
    bout << "0.000000 0.000000 1.000000"                            << "\n";
    bout << "1.000000 0.000000 1.000000"                            << "\n";
    bout << "0.000000 1.000000 1.000000"                            << "\n";
    bout << "1.000000 1.000000 1.000000"                            << "\n";

    OCIO::BakerRcPtr baker = OCIO::Baker::Create();
    baker->setConfig(config);
    std::ostringstream metadata;
    metadata << "Alexa conversion LUT, logc2video. Full in/full out." << "\n";
    metadata << "created by alexalutconv (2.11)" << "\n";
    baker->setMetadata(metadata.str().c_str());
    baker->setFormat("iridas_cube");
    baker->setInputSpace("lnf");
    baker->setTargetSpace("target");
    baker->setCubeSize(2);
    std::ostringstream output;
    baker->bake(output);

    //
    std::vector<std::string> osvec;
    pystring::splitlines(output.str(), osvec);
    std::vector<std::string> resvec;
    pystring::splitlines(bout.str(), resvec);
    OCIO_CHECK_EQUAL(osvec.size(), resvec.size());
    for(unsigned int i = 0; i < resvec.size(); ++i)
    {
        OCIO_CHECK_EQUAL(osvec[i], resvec[i]);
    }
}

#endif // OCIO_UNIT_TEST
