
# Core API

**Usage Example:** *Compositing plugin that converts from "log" to "lin"*

.. code-block:: cpp
   
   #include <OpenColorIO/OpenColorIO.h>
   namespace OCIO = OCIO_NAMESPACE;
   
   try
   {
       // Get the global OpenColorIO config
       // This will auto-initialize (using $OCIO) on first use
       OCIO::ConstConfigRcPtr config = OCIO::GetCurrentConfig();
       
       // Get the processor corresponding to this transform.
       OCIO::ConstProcessorRcPtr processor = config->getProcessor(OCIO::ROLE_COMPOSITING_LOG,
                                                                  OCIO::ROLE_SCENE_LINEAR);
    
       // Wrap the image in a light-weight ImageDescription
       OCIO::PackedImageDesc img(imageData, w, h, 4);
       
       // Apply the color transformation (in place)
       processor->apply(img);
   }
   catch(OCIO::Exception & exception)
   {
       std::cerr << "OpenColorIO Error: " << exception.what() << std::endl;
   }




## Exceptions

<!-- tabs:start -->

#### **C++**


Exception
An exception class to throw for errors detected at   runtime.      .. warning::       All functions in the Config class can potentially throw this exception.
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>explicit Exception::Exception(const char *)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Constructor that takes a string as the exception message.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>Exception::Exception(const Exception &)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Constructor that takes an existing exception.</td>
</tr>
</table>
ExceptionMissingFile
An exception class for errors detected at   runtime, thrown when OCIO cannot find a file that is expected to   exist. This is provided as a custom type to   distinguish cases where one wants to continue looking for   missing files, but wants to properly fail   for other error conditions.
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>explicit ExceptionMissingFile::ExceptionMissingFile(const char *)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Constructor that takes a string as the exception message.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ExceptionMissingFile::ExceptionMissingFile(const ExceptionMissingFile &)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Constructor that takes an existing exception.</td>
</tr>
</table>
<!-- tabs:end -->

## Global

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>extern void ClearAllCaches()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>OpenColorIO, during normal usage, tends to cache certain information   (such as the contents of LUTs on disk, intermediate results, etc.).   Calling this function will flush all such information.   Under normal usage, this is not necessary, but it can be helpful in particular instances,   such as designing OCIO profiles, and wanting to re-read luts without   restarting.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>extern const char * GetVersion()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the version number for the library, as a   dot-delimited string (e.g., "1.0.0"). This is also available   at compile time as OCIO_VERSION.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>extern int GetVersionHex()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the version number for the library, as a   single 4-byte hex number (e.g., 0x01050200 for "1.5.2"), to be used   for numeric comparisons. This is also available   at compile time as OCIO_VERSION_HEX.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>extern LoggingLevel GetLoggingLevel()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the global logging level.   You can override this at runtime using the `OCIO_LOGGING_LEVEL`   environment variable. The client application that sets this should use   `SetLoggingLevel`, and not the environment variable. The default value is INFO.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>extern void SetLoggingLevel(LoggingLevel level)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Set the global logging level.</td>
</tr>
</table>
<!-- tabs:end -->

## Config

A config defines all the color spaces to be available at runtime.

The color configuration (`Config`) is the main object for
interacting with this library. It encapsulates all of the information
necessary to use customized `ColorSpaceTransform` and
`DisplayTransform` operations.

See the `user-guide` for more information on
selecting, creating, and working with custom color configurations.

For applications interested in using only one color config at
a time (this is the vast majority of apps), their API would
traditionally get the global configuration and use that, as opposed to
creating a new one. This simplifies the use case for
plugins and bindings, as it alleviates the need to pass around configuration
handles.

An example of an application where this would not be sufficient would be
a multi-threaded image proxy server (daemon), which wished to handle
multiple show configurations in a single process concurrently. This
app would need to keep multiple configurations alive, and to manage them
appropriately.

Roughly speaking, a novice user should select a
default configuration that most closely approximates the use case
(animation, visual effects, etc.), and set the `OCIO` environment
variable to point at the root of that configuration.

?> **Note:**
   Initialization using environment variables is typically preferable in
   a multi-app ecosystem, as it allows all applications to be
   consistently configured.

See `developers-usageexamples`

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>extern ConstConfigRcPtr GetCurrentConfig()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the current configuration.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>extern void SetCurrentConfig(const ConstConfigRcPtr & config)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Set the current configuration. This will then store a copy of the specified config.</td>
</tr>
</table>
Config

<!-- tabs:end -->

### Initialization

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static ConfigRcPtr Config::Create()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Constructor a default empty configuration.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static ConstConfigRcPtr Config::CreateFromEnv()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Constructor a configuration using the OCIO environmnet variable.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static ConstConfigRcPtr Config::CreateFromFile(const char * filename)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Constructor a configuration using a specific config file.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static ConstConfigRcPtr Config::CreateFromStream(std::istream & istream)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConfigRcPtr Config::createEditableCopy() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>unsigned int Config::getMajorVersion() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the configuration major version</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setMajorVersion(unsigned int major)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Set the configuration major version</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>unsigned int Config::getMinorVersion() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the configuration minor version</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setMinorVersion(unsigned int minor)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Set the configuration minor version</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::sanityCheck() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>This will throw an exception if the config is malformed. The most   common error occurs when references are made to colorspaces that do not   exist.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getDescription() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setDescription(const char * description)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::serialize(std::ostream & os) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Returns the string representation of the Config in YAML text form.   This is typically stored on disk in a file with the extension .ocio.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getCacheID() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>This will produce a hash of the all colorspace definitions, etc.   All external references, such as files used in FileTransforms, etc.,   will be incorporated into the cacheID. While the contents of   the files are not read, the file system is queried for relavent   information (mtime, inode) so that the config's cacheID will   change when the underlying luts are updated.   If a context is not provided, the current Context will be used.   If a null context is provided, file references will not be taken into   account (this is essentially a hash of Config::serialize).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getCacheID(const ConstContextRcPtr & context) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>

#### **Python**

Class `PyOpenColorIO.Config`


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>Config()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Create a default empty configuration.</td>
</tr>
<tr>
    <th scope="row">Parameters</th>
    <td>None</td>
</tr>
<tr>
    <th scope="row">Returns</th>
    <td><code>PyOpenColorIO.Config<code></td>
</tr>
</table>


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>Config().CreateFromEnv()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Create a PyOpenColorIO.Config object using the $OCIO environment variable.</td>
</tr>
<tr>
    <th scope="row">Parameters</th>
    <td>None</td>
</tr>
<tr>
    <th scope="row">Returns</th>
    <td><code>PyOpenColorIO.Config<code></td>
</tr>
</table>


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>Config().CreateFromFile(filename)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Create a PyOpenColorIO.Config object using the information in a file.</td>
</tr>
<tr>
    <th scope="row">Parameters</th>
    <td><b>filename</b> (<code>string</code>)</td>
</tr>
<tr>
    <th scope="row">Returns</th>
    <td><code>PyOpenColorIO.Config<code></td>
</tr>
</table>
<!-- tabs:end -->

### Resources
Given a lut src name, where should we find it?

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstContextRcPtr Config::getCurrentContext() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::addEnvironmentVar(const char * name, const char * defaultValue)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Config::getNumEnvironmentVars() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getEnvironmentVarNameByIndex(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getEnvironmentVarDefault(const char * name) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::clearEnvironmentVars()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setEnvironmentMode(EnvironmentMode mode)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>EnvironmentMode Config::getEnvironmentMode() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::loadEnvironment()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getSearchPath() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setSearchPath(const char * path)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Set all search paths as a concatenated string, using   ':' to separate the paths.  See addSearchPath for a more robust and   platform-agnostic method of setting the search paths.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Config::getNumSearchPaths() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getSearchPath(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get a search path from the list. The paths are in the   order they will be searched (that is, highest to lowest priority).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::clearSearchPaths()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::addSearchPath(const char * path)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Add a single search path to the end of the list.   Paths may be either absolute or relative. Relative paths are   relative to the working directory. Forward slashes will be   normalized to reverse for Windows. Environment (context) variables   may be used in paths.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getWorkingDir() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setWorkingDir(const char * dirname)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>The working directory defaults to the location of the   config file. It is used to convert any relative paths to absolute.   If no search paths have been set, the working directory will be used   as the fallback search path. No environment (context) variables may   be used in the working directory.</td>
</tr>
</table>
<!-- tabs:end -->

### ColorSpaces

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ColorSpaceSetRcPtr Config::getColorSpaces(const char * category) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get all color spaces having a specific category   in the order they appear in the config file.      ?> **Note:**      If the category is null or empty, the method returns       all the color spaces like `Config::getNumColorSpaces`       and `Config::getColorSpaceNameByIndex` do.      ?> **Note:**      It's worth noticing that the method returns a copy of the       selected color spaces decoupling the result from the config.       Hence, any changes on the config do not affect the existing       color space sets, and vice-versa.      ?> **Note:**      These fcns all accept either a color space OR role name.      (Color space names take precedence over roles.)</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Config::getNumColorSpaces() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getColorSpaceNameByIndex(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Will be null for invalid index.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstColorSpaceRcPtr Config::getColorSpace(const char * name) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Will return null if the name is not found.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Config::getIndexForColorSpace(const char * name) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Will return -1 if the name is not found.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::addColorSpace(const ConstColorSpaceRcPtr & cs)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Add a color space to the configuration.      ?> **Note:**      If another color space is already registered with the same name,      this will overwrite it. This stores a copy of the specified      color space.   ?> **Note:**      Adding a color space to a Config does not affect any ColorSpaceSets       that have already been created.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::clearColorSpaces()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Remove all the color spaces from the configuration.      ?> **Note:**      Removing color spaces from a Config does not affect       any ColorSpaceSets that have already been created.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::parseColorSpaceFromString(const char * str) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Given the specified string, get the longest,   right-most, colorspace substring that appears.      * If strict parsing is enabled, and no color space is found, return     an empty string.   * If strict parsing is disabled, return ROLE_DEFAULT (if defined).   * If the default role is not defined, return an empty string.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool Config::isStrictParsingEnabled() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setStrictParsingEnabled(bool enabled)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

### Roles
A role is like an alias for a colorspace. You can query the colorspace
corresponding to a role using the normal getColorSpace fcn.

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setRole(const char * role, const char * colorSpaceName)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>?> **Note:**      Setting the ``colorSpaceName`` name to a null string unsets it.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Config::getNumRoles() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool Config::hasRole(const char * role) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Return true if the role has been defined.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getRoleName(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the role name at index, this will return values   like 'scene_linear', 'compositing_log'.   Return empty string if index is out of range.</td>
</tr>
</table>
<!-- tabs:end -->

### Display/View Registration

Looks is a potentially comma (or colon) delimited list of lookNames,
Where +/- prefixes are optionally allowed to denote forward/inverse
look specification. (And forward is assumed in the absence of either)

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getDefaultDisplay() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Config::getNumDisplays() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getDisplay(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getDefaultView(const char * display) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Config::getNumViews(const char * display) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getView(const char * display, int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getDisplayColorSpaceName(const char * display, const char * view) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getDisplayLooks(const char * display, const char * view) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::addDisplay(const char * display, const char * view, const char * colorSpaceName, const char * looks)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>For the (display,view) combination,   specify which colorSpace and look to use.   If a look is not desired, then just pass an empty string</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::clearDisplays()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setActiveDisplays(const char * displays)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Comma-delimited list of display names.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getActiveDisplays() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setActiveViews(const char * views)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Comma-delimited list of view names.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getActiveViews() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

### Luma

Get the default coefficients for computing luma.

?> **Note:**
   There is no "1 size fits all" set of luma coefficients. (The
   values are typically different for each colorspace, and the
   application of them may be nonsensical depending on the
   intensity coding anyways). Thus, the 'right' answer is to make
   these functions on the `Config` class. However, it's
   often useful to have a config-wide default so here it is. We will
   add the colorspace specific luma call if/when another client is
   interesting in using it.

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::getDefaultLumaCoefs(float * rgb) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setDefaultLumaCoefs(const float * rgb)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>These should be normalized (sum to 1.0 exactly).</td>
</tr>
</table>
<!-- tabs:end -->

### Look

Manager per-shot look settings.


<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstLookRcPtr Config::getLook(const char * name) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Config::getNumLooks() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Config::getLookNameByIndex(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::addLook(const ConstLookRcPtr & look)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::clearLooks()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

### Processors

Create a `Processor` to assemble a transformation between two 
color spaces.  It may then be used to create a `CPUProcessor` 
or `GPUProcessor` to process/convert pixels.

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstProcessorRcPtr Config::getProcessor(const ConstContextRcPtr & context, const ConstColorSpaceRcPtr & srcColorSpace, const ConstColorSpaceRcPtr & dstColorSpace) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstProcessorRcPtr Config::getProcessor(const ConstColorSpaceRcPtr & srcColorSpace, const ConstColorSpaceRcPtr & dstColorSpace) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstProcessorRcPtr Config::getProcessor(const char * srcName, const char * dstName) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>?> **Note:**      Names can be colorspace name, role name, or a combination of both.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstProcessorRcPtr Config::getProcessor(const ConstContextRcPtr & context, const char * srcName, const char * dstName) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the processor for the specified transform.      Not often needed, but will allow for the re-use of atomic OCIO   functionality (such as to apply an individual LUT file).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstProcessorRcPtr Config::getProcessor(const ConstTransformRcPtr& transform) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstProcessorRcPtr Config::getProcessor(const ConstTransformRcPtr& transform, TransformDirection direction) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstProcessorRcPtr Config::getProcessor(const ConstContextRcPtr & context, const ConstTransformRcPtr& transform, TransformDirection direction) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->
.. _colorspace_section:


## ColorSpace
The *ColorSpace* is the state of an image with respect to colorimetry
and color encoding. Transforming images between different
*ColorSpaces* is the primary motivation for this library.

While a complete discussion of color spaces is beyond the scope of
header documentation, traditional uses would be to have *ColorSpaces*
corresponding to: physical capture devices (known cameras, scanners),
and internal 'convenience' spaces (such as scene linear, logarithmic).

*ColorSpaces* are specific to a particular image precision (float32,
uint8, etc.), and the set of ColorSpaces that provide equivalent mappings
(at different precisions) are referred to as a 'family'.

<!-- tabs:start -->

#### **C++**


ColorSpace

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static ColorSpaceRcPtr ColorSpace::Create()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ColorSpaceRcPtr ColorSpace::createEditableCopy() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * ColorSpace::getName() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::setName(const char * name)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * ColorSpace::getFamily() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the family, for use in user interfaces (optional)</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::setFamily(const char * family)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Set the family, for use in user interfaces (optional)</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * ColorSpace::getEqualityGroup() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the ColorSpace group name (used for equality comparisons)   This allows no-op transforms between different colorspaces.   If an equalityGroup is not defined (an empty string), it will be considered   unique (i.e., it will not compare as equal to other ColorSpaces with an   empty equality group).  This is often, though not always, set to the   same value as 'family'.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::setEqualityGroup(const char * equalityGroup)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * ColorSpace::getDescription() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::setDescription(const char * description)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>BitDepth ColorSpace::getBitDepth() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::setBitDepth(BitDepth bitDepth)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

### Categories

A category is used to allow applications to filter the list of color spaces 
they display in menus based on what that color space is used for.

Here is an example config entry that could appear under a ColorSpace:
categories: [input, rendering]

The example contains two categories: 'input' and 'rendering'. 
Category strings are not case-sensitive and the order is not significant.
There is no limit imposed on length or number. Although users may add 
their own categories, the strings will typically come from a fixed set 
listed in the documentation (similar to roles).

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool ColorSpace::hasCategory(const char * category) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Return true if the category is present.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::addCategory(const char * category)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Add a single category.   ?> **Note:** Will do nothing if the category already exists.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::removeCategory(const char * category)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Remove a category.   ?> **Note:** Will do nothing if the category is missing.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int ColorSpace::getNumCategories() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the number of categories.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * ColorSpace::getCategory(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Return the category name using its index   ?> **Note:** Will be null if the index is invalid.</td>
</tr>
</table>
<!-- tabs:end -->

### Data

ColorSpaces that are data are treated a bit special. Basically, any
colorspace transforms you try to apply to them are ignored. (Think
of applying a gamut mapping transform to an ID pass). Also, the
`DisplayTransform` process obeys special 'data min' and
'data max' args.

This is traditionally used for pixel data that represents non-color
pixel data, such as normals, point positions, ID information, etc.

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool ColorSpace::isData() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::setIsData(bool isData)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

### Allocation

If this colorspace needs to be transferred to a limited dynamic
range coding space (such as during display with a GPU path), use this
allocation to maximize bit efficiency.

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>Allocation ColorSpace::getAllocation() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::setAllocation(Allocation allocation)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Specify the optional variable values to configure the allocation.   If no variables are specified, the defaults are used.      ALLOCATION_UNIFORM::            2 vars: [min, max]      ALLOCATION_LG2::            2 vars: [lg2min, lg2max]      3 vars: [lg2min, lg2max, linear_offset]</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int ColorSpace::getAllocationNumVars() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::getAllocationVars(float * vars) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::setAllocationVars(int numvars, const float * vars)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

### Transform

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstTransformRcPtr ColorSpace::getTransform(ColorSpaceDirection dir) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>If a transform in the specified direction has been specified,   return it. Otherwise return a null ConstTransformRcPtr</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpace::setTransform(const ConstTransformRcPtr & transform, ColorSpaceDirection dir)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Specify the transform for the appropriate direction.   Setting the transform to null will clear it.</td>
</tr>
</table>
<!-- tabs:end -->

## ColorSpaceSet

The *ColorSpaceSet* is a set of color spaces (i.e. no color space duplication) 
which could be the result of `Config::getColorSpaces`
or built from scratch.

?> **Note:**
   The color spaces are decoupled from the config ones, i.e., any 
   changes to the set itself or to its color spaces do not affect the 
   original color spaces from the configuration.  If needed, 
   use `Config::addColorSpace` to update the configuration.

<!-- tabs:start -->

#### **C++**


ColorSpaceSet

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static ColorSpaceSetRcPtr ColorSpaceSet::Create()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Create an empty set of color spaces.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ColorSpaceSetRcPtr ColorSpaceSet::createEditableCopy() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Create a set containing a copy of all the color spaces.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool ColorSpaceSet::operator==(const ColorSpaceSet & css) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Return true if the two sets are equal.   ?> **Note:** The comparison is done on the color space names (not a deep comparison).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool ColorSpaceSet::operator!=(const ColorSpaceSet & css) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Return true if the two sets are different.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int ColorSpaceSet::getNumColorSpaces() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Return the number of color spaces.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * ColorSpaceSet::getColorSpaceNameByIndex(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Return the color space name using its index.   This will be null if an invalid index is specified.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstColorSpaceRcPtr ColorSpaceSet::getColorSpaceByIndex(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Return the color space using its index.   This will be empty if an invalid index is specified.</td>
</tr>
</table>
<!-- tabs:end -->
<!-- tabs:start -->

#### **C++**

?> **Note:**   These fcns only accept color space names (i.e. no role name).
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstColorSpaceRcPtr ColorSpaceSet::getColorSpace(const char * name) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Will return null if the name is not found.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int ColorSpaceSet::getIndexForColorSpace(const char * name) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Will return -1 if the name is not found.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpaceSet::addColorSpace(const ConstColorSpaceRcPtr & cs)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Add color space(s).      ?> **Note:**      If another color space is already registered with the same name,      this will overwrite it. This stores a copy of the specified      color space(s).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpaceSet::removeColorSpace(const char * name)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Remove color space(s) using color space names (i.e. no role name).      ?> **Note:**      The removal of a missing color space does nothing.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ColorSpaceSet::clearColorSpaces()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Clear all color spaces.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstColorSpaceSetRcPtr operator||(const ConstColorSpaceSetRcPtr & lcss,  const ConstColorSpaceSetRcPtr & rcss)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Perform the union of two sets.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstColorSpaceSetRcPtr operator&&(const ConstColorSpaceSetRcPtr & lcss,  const ConstColorSpaceSetRcPtr & rcss)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Perform the intersection of two sets.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstColorSpaceSetRcPtr operator-(const ConstColorSpaceSetRcPtr & lcss,  const ConstColorSpaceSetRcPtr & rcss)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Perform the difference of two sets.</td>
</tr>
</table>
<!-- tabs:end -->

## Look

The *Look* is an 'artistic' image modification, in a specified image
state.
The processSpace defines the ColorSpace the image is required to be
in, for the math to apply correctly.

<!-- tabs:start -->

#### **C++**


Look

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static LookRcPtr Look::Create()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>LookRcPtr Look::createEditableCopy() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Look::getName() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Look::setName(const char * name)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Look::getProcessSpace() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Look::setProcessSpace(const char * processSpace)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstTransformRcPtr Look::getTransform() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Look::setTransform(const ConstTransformRcPtr & transform)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Setting a transform to a non-null call makes it allowed.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstTransformRcPtr Look::getInverseTransform() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Look::setInverseTransform(const ConstTransformRcPtr & transform)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Setting a transform to a non-null call makes it allowed.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Look::getDescription() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Look::setDescription(const char * description)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

## Processor

The *Processor* represents a specific color transformation which is 
the result of `Config::getProcessor`.

<!-- tabs:start -->

#### **C++**


Processor

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool Processor::isNoOp() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool Processor::hasChannelCrosstalk() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>True if the image transformation is non-separable.   For example, if a change in red may also cause a change in green or blue.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstProcessorMetadataRcPtr Processor::getProcessorMetadata() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>The ProcessorMetadata contains technical information                  such as the number of files and looks used in the processor.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const FormatMetadata & Processor::getFormatMetadata() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get a FormatMetadata containing the top level metadata                  for the processor.  For a processor from a CLF file,                  this corresponds to the ProcessList metadata.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Processor::getNumTransforms() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the number of transforms that comprise the processor.                  Each transform has a (potentially empty) FormatMetadata.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const FormatMetadata & Processor::getTransformFormatMetadata(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get a FormatMetadata containing the metadata for a                  transform within the processor. For a processor from                  a CLF file, this corresponds to the metadata associated                  with an individual process node.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>GroupTransformRcPtr Processor::createGroupTransform() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Return a cpp:class:`GroupTransform` that contains a                  copy of the transforms that comprise the processor.                  (Changes to it will not modify the original processor.)</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Processor::write(const char * formatName, std::ostream & os) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Write the transforms comprising the processor to the stream.                  Writing (as opposed to Baking) is a lossless process.                  An exception is thrown if the processor cannot be                  losslessly written to the specified file format.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static int Processor::getNumWriteFormats()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the number of writers.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static const char * Processor::getFormatNameByIndex(int index)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the writer at index, return empty string if                  an invalid index is specified.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>DynamicPropertyRcPtr Processor::getDynamicProperty(DynamicPropertyType type) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Processor::getCacheID() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->
GPU Renderer
^^^^^^^^^^^^
<!-- tabs:start -->

#### **C++**

Get an optimized `GPUProcessor` instance.
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstGPUProcessorRcPtr Processor::getDefaultGPUProcessor() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstGPUProcessorRcPtr Processor::getOptimizedGPUProcessor(OptimizationFlags oFlags,  FinalizationFlags fFlags) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

### CPU Renderer

Get an optimized `CPUProcessor` instance.
       
?> **Note:**
   This may provide higher fidelity than anticipated due to internal
   optimizations. For example, if the inputColorSpace and the
   outputColorSpace are members of the same family, no conversion
   will be applied, even though strictly speaking quantization
   should be added.

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstCPUProcessorRcPtr Processor::getDefaultCPUProcessor() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstCPUProcessorRcPtr Processor::getOptimizedCPUProcessor(OptimizationFlags oFlags,  FinalizationFlags fFlags) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstCPUProcessorRcPtr Processor::getOptimizedCPUProcessor(BitDepth inBitDepth, BitDepth outBitDepth, OptimizationFlags oFlags,  FinalizationFlags fFlags) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

### CPUProcessor

<!-- tabs:start -->

#### **C++**


CPUProcessor

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * CPUProcessor::getCacheID() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool CPUProcessor::hasChannelCrosstalk() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>BitDepth CPUProcessor::getInputBitDepth() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Bit-depths of the input and output pixel buffers.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>DynamicPropertyRcPtr CPUProcessor::getDynamicProperty(DynamicPropertyType type) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Refer to `GPUProcessor::getDynamicProperty`.</td>
</tr>
</table>
<!-- tabs:end -->
Apply to an image with any kind of channel ordering while respecting 
<!-- tabs:start -->

#### **C++**

the input and output bit-depths.
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void CPUProcessor::apply(ImageDesc & imgDesc) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void CPUProcessor::apply(const ImageDesc & srcImgDesc, ImageDesc & dstImgDesc) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->
Apply to a single pixel respecting that the input and output bit-depths
be identical.

<!-- tabs:start -->

#### **C++**

?> **Note:**   This is not as efficient as applying to an entire image at once.   If you are processing multiple pixels, and have the flexibility,   use the above function instead.
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void CPUProcessor::applyRGB(void * pixel) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void CPUProcessor::applyRGBA(void * pixel) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

### GPUProcessor

<!-- tabs:start -->

#### **C++**


GPUProcessor

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool GPUProcessor::isNoOp() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * GPUProcessor::getCacheID() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>bool GPUProcessor::hasChannelCrosstalk() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>DynamicPropertyRcPtr GPUProcessor::getDynamicProperty(DynamicPropertyType type) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>The returned pointer may be used to set the value of any                  dynamic properties of the requested type.  Throws if the                  requested property is not found.  Note that if the                  processor contains several ops that support the                  requested property, only ones for which dynamic has                  been enabled will be controlled.      ?> **Note:**       The dynamic properties in this object are decoupled from the ones       in the `Processor` it was generated from.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void GPUProcessor::extractGpuShaderInfo(GpuShaderDescRcPtr & shaderDesc) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Extract the shader information to implement the color processing.</td>
</tr>
</table>
ProcessorMetadata
This class contains meta information about the process that generated   this processor.  The results of these functions do not   impact the pixel processing.
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static ProcessorMetadataRcPtr ProcessorMetadata::Create()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int ProcessorMetadata::getNumFiles() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * ProcessorMetadata::getFile(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int ProcessorMetadata::getNumLooks() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * ProcessorMetadata::getLook(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ProcessorMetadata::addFile(const char * fname)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void ProcessorMetadata::addLook(const char * look)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

## Baker

In certain situations it is necessary to serialize transforms into a variety
of application specific lut formats. The Baker can be used to create lut
formats that ocio supports for writing.

**Usage Example:** *Bake a houdini sRGB viewer lut*

.. code-block:: cpp
   
   OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromEnv();
   OCIO::BakerRcPtr baker = OCIO::Baker::Create();
   baker->setConfig(config);
   baker->setFormat("houdini"); // set the houdini type
   baker->setType("3D"); // we want a 3D lut
   baker->setInputSpace("lnf");
   baker->setShaperSpace("log");
   baker->setTargetSpace("sRGB");
   std::ostringstream out;
   baker->bake(out); // fresh bread anyone!
   std::cout << out.str();

<!-- tabs:start -->

#### **C++**


<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static BakerRcPtr Create()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>create a new Baker</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>BakerRcPtr createEditableCopy() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>create a copy of this Baker</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void setConfig(const ConstConfigRcPtr & config)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>set the config to use</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ConstConfigRcPtr getConfig() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the config to use</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void setFormat(const char * formatName)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>set the lut output format</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * getFormat() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the lut output format</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void setType(const char * type)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>set the lut output type (1D or 3D)</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * getType() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the lut output type</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void setMetadata(const char * metadata)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>set *optional* meta data for luts that support it</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * getMetadata() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the meta data that has been set</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void setInputSpace(const char * inputSpace)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>set the input ColorSpace that the lut will be   applied to</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * getInputSpace() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the input ColorSpace that has been set</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void setShaperSpace(const char * shaperSpace)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>set an *optional* ColorSpace to be used to shape /   transfer the input colorspace. This is mostly used to allocate   an HDR luminance range into an LDR one. If a shaper space   is not explicitly specified, and the file format supports one,   the ColorSpace Allocation will be used</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * getShaperSpace() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the shaper colorspace that has been set</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void setLooks(const char * looks)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>set the looks to be applied during baking   Looks is a potentially comma (or colon) delimited list of lookNames,   Where +/- prefixes are optionally allowed to denote forward/inverse   look specification. (And forward is assumed in the absence of either)</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * getLooks() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the looks to be applied during baking</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void setTargetSpace(const char * targetSpace)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>set the target device colorspace for the lut</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * getTargetSpace() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the target colorspace that has been set</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void setShaperSize(int shapersize)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>override the default the shaper sample size,   default: <format specific></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int getShaperSize() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the shaper sample size</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void setCubeSize(int cubesize)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>override the default cube sample size   default: <format specific></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int getCubeSize() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the cube sample size</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void bake(std::ostream & os) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>bake the lut into the output stream</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static int getNumFormats()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the number of lut writers</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static const char * getFormatNameByIndex(int index)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>get the lut writer at index, return empty string if   an invalid index is specified</td>
</tr>
</table>
<!-- tabs:end -->

## ImageDesc


.. c:var:: const ptrdiff_t AutoStride
   
   AutoStride

<!-- tabs:start -->

#### **C++**


ImageDesc
This is a light-weight wrapper around an image, that provides a context   for pixel access. This does NOT claim ownership of the pixels or copy   image data
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual ImageDesc::~ImageDesc()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

## PackedImageDesc

<!-- tabs:start -->

#### **C++**


PackedImageDesc

<!-- tabs:end -->
All the constructors expect a pointer to packed image data (such as 
rgbrgbrgb or rgbargbargba) starting at the first color channel of 
the first pixel to process (which does not need to be the first pixel 
of the image). The number of channels must be greater than or equal to 3.
If a 4th channel is specified, it is assumed to be alpha
information.  Channels > 4 will be ignored.

?> **Note:** 
<!-- tabs:start -->

#### **C++**

The methods assume the CPUProcessor bit-depth type for the data pointer.
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>PackedImageDesc::PackedImageDesc(void * data, long width, long height, long numChannels)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>?> **Note:**       numChannels must be 3 (RGB) or 4 (RGBA).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>PackedImageDesc::PackedImageDesc(void * data, long width, long height, long numChannels, ptrdiff_t chanStrideBytes, ptrdiff_t xStrideBytes, ptrdiff_t yStrideBytes)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>?> **Note:**       numChannels smust be 3 (RGB) or 4 (RGBA).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>PackedImageDesc::PackedImageDesc(void * data, long width, long height, ChannelOrdering chanOrder)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>PackedImageDesc::PackedImageDesc(void * data, long width, long height, ChannelOrdering chanOrder, ptrdiff_t chanStrideBytes, ptrdiff_t xStrideBytes, ptrdiff_t yStrideBytes)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual PackedImageDesc::~PackedImageDesc()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ChannelOrdering PackedImageDesc::getChannelOrder() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the channel ordering of all the pixels.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void * PackedImageDesc::getData() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get a pointer to the first color channel of the first pixel.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>long PackedImageDesc::getWidth() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the width to process (where x position starts at 0 and ends at width-1).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>long PackedImageDesc::getHeight() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the height to process (where y position starts at 0 and ends at height-1).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>long PackedImageDesc::getNumChannels() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the number of color channels.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ptrdiff_t PackedImageDesc::getChanStrideBytes() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the step in bytes to find the next color channel in the same pixel.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ptrdiff_t PackedImageDesc::getXStrideBytes() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the step in bytes to find the same color channel in the next pixel.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ptrdiff_t PackedImageDesc::getYStrideBytes() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the step in bytes to find the same color channel   of the pixel at the same position in the next line.</td>
</tr>
</table>
<!-- tabs:end -->

## PlanarImageDesc

<!-- tabs:start -->

#### **C++**


PlanarImageDesc

<!-- tabs:end -->
All the constructors expect pointers to the specified image planes 
(i.e. rrrr gggg bbbb) starting at the first color channel of the 
first pixel to process (which need not be the first pixel of the image).
Pass NULL for aData if no alpha exists (r/g/bData must not be NULL).

?> **Note:** 
<!-- tabs:start -->

#### **C++**

The methods assume the CPUProcessor bit-depth type for the R/G/B/A data pointers.
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>PlanarImageDesc::PlanarImageDesc(void * rData, void * gData, void * bData, void * aData, long width, long height)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>PlanarImageDesc::PlanarImageDesc(void * rData, void * gData, void * bData, void * aData, long width, long height, ptrdiff_t xStrideBytes, ptrdiff_t yStrideBytes)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual PlanarImageDesc::~PlanarImageDesc()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void * PlanarImageDesc::getRData() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get a pointer to the red channel of the first pixel.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void * PlanarImageDesc::getGData() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get a pointer to the green channel of the first pixel.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void * PlanarImageDesc::getBData() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get a pointer to the blue channel of the first pixel.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void * PlanarImageDesc::getAData() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get a pointer to the alpha channel of the first pixel   or null as alpha channel is optional.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>long PlanarImageDesc::getWidth() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the width to process (where x position starts at 0 and ends at width-1).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>long PlanarImageDesc::getHeight() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the height to process (where y position starts at 0 and ends at height-1).</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ptrdiff_t PlanarImageDesc::getXStrideBytes() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the step in bytes to find the same color channel of the next pixel.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ptrdiff_t PlanarImageDesc::getYStrideBytes() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the step in bytes to find the same color channel   of the pixel at the same position in the next line.</td>
</tr>
</table>
<!-- tabs:end -->

## GpuShaderDesc

This class holds the GPU-related information needed to build a shader program
from a specific processor.

This class defines the interface and there are two implementations provided.
The "legacy" mode implements the OCIO v1 approach of baking certain ops
in order to have at most one 3D-LUT.  The "generic" mode is the v2 default and 
allows all the ops to be processed as-is, without baking, like the CPU renderer.
Custom implementations could be written to accomodate the GPU needs of a 
specific client app.


The complete fragment shader program is decomposed in two main parts:
the OCIO shader program for the color processing and the client shader 
program which consumes the pixel color processing.
   
The OCIO shader program is fully described by the GpuShaderDesc
independently from the client shader program. The only critical 
point is the agreement on the OCIO function shader name.

To summarize, the complete shader program is:
   
.. code-block:: cpp
 
 ////////////////////////////////////////////////////////////////////////
 //                                                                    //
 //               The complete fragment shader program                 //
 //                                                                    //
 ////////////////////////////////////////////////////////////////////////
 //                                                                    //
 //   //////////////////////////////////////////////////////////////   //
 //   //                                                          //   // 
 //   //               The OCIO shader program                    //   //
 //   //                                                          //   //
 //   //////////////////////////////////////////////////////////////   //
 //   //                                                          //   //
 //   //   // All global declarations                             //   //
 //   //   uniform sampled3D tex3;                                //   //
 //   //                                                          //   //
 //   //   // All helper methods                                  //   //
 //   //   vec3 computePos(vec3 color)                            //   //
 //   //   {                                                      //   //
 //   //      vec3 coords = color;                                //   //
 //   //      ...                                                 //   //
 //   //      return coords;                                      //   //
 //   //   }                                                      //   //
 //   //                                                          //   //
 //   //   // The OCIO shader function                            //   //
 //   //   vec4 OCIODisplay(in vec4 inColor)                      //   //
 //   //   {                                                      //   //
 //   //      vec4 outColor = inColor;                            //   //
 //   //      ...                                                 //   //
 //   //      outColor.rbg                                        //   //
 //   //         = texture3D(tex3, computePos(inColor.rgb)).rgb;  //   //
 //   //      ...                                                 //   //
 //   //      return outColor;                                    //   //
 //   //   }                                                      //   //
 //   //                                                          //   //
 //   //////////////////////////////////////////////////////////////   //
 //                                                                    //
 //   //////////////////////////////////////////////////////////////   //
 //   //                                                          //   // 
 //   //             The client shader program                    //   //
 //   //                                                          //   //
 //   //////////////////////////////////////////////////////////////   //
 //   //                                                          //   //
 //   //   uniform sampler2D image;                               //   //
 //   //                                                          //   //
 //   //   void main()                                            //   //
 //   //   {                                                      //   //
 //   //      vec4 inColor = texture2D(image, gl_TexCoord[0].st); //   //
 //   //      ...                                                 //   //
 //   //      vec4 outColor = OCIODisplay(inColor);               //   //
 //   //      ...                                                 //   //
 //   //      gl_FragColor = outColor;                            //   //
 //   //   }                                                      //   //
 //   //                                                          //   //
 //   //////////////////////////////////////////////////////////////   //
 //                                                                    //
 ////////////////////////////////////////////////////////////////////////


**Usage Example:** *Building a GPU shader*

  This example is based on the code in: src/apps/ociodisplay/main.cpp 

.. code-block:: cpp

   // Get the processor
   //
   OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromEnv();
   OCIO::ConstProcessorRcPtr processor 
      = config->getProcessor("ACES - ACEScg", "Output - sRGB");

   // Step 1: Create a GPU shader description
   //
   // The three potential scenarios are:
   //
   //   1. Instantiate the legacy shader description.  The color processor
   //      is baked down to contain at most one 3D LUT and no 1D LUTs.
   //
   //      This is the v1 behavior and will remain part of OCIO v2
   //      for backward compatibility.
   //
   OCIO::GpuShaderDescRcPtr shaderDesc
         = OCIO::GpuShaderDesc::CreateLegacyShaderDesc(LUT3D_EDGE_SIZE);
   //
   //   2. Instantiate the generic shader description.  The color processor 
   //      is used as-is (i.e. without any baking step) and could contain 
   //      any number of 1D & 3D luts.
   //
   //      This is the default OCIO v2 behavior and allows a much better
   //      match between the CPU and GPU renderers.
   //
   OCIO::GpuShaderDescRcPtr shaderDesc = OCIO::GpuShaderDesc::Create();
   //
   //   3. Instantiate a custom shader description.
   //
   //      Writing a custom shader description is a way to tailor the shaders
   //      to the needs of a given client program.  This involves writing a
   //      new class inheriting from the pure virtual class GpuShaderDesc.
   //
   //      Please refer to the GenericGpuShaderDesc class for an example.
   //
   OCIO::GpuShaderDescRcPtr shaderDesc = MyCustomGpuShader::Create();

   shaderDesc->setLanguage(OCIO::GPU_LANGUAGE_GLSL_1_3);
   shaderDesc->setFunctionName("OCIODisplay");

   // Step 2: Collect the shader program information for a specific processor
   //
   processor->extractGpuShaderInfo(shaderDesc);

   // Step 3: Create a helper to build the shader. Here we use a helper for
   //         OpenGL but there will also be helpers for other languages.
   //
   OpenGLBuilderRcPtr oglBuilder = OpenGLBuilder::Create(shaderDesc);

   // Step 4: Allocate & upload all the LUTs
   //
   oglBuilder->allocateAllTextures();

   // Step 5: Build the complete fragment shader program using 
   //         g_fragShaderText which is the client shader program.
   //
   g_programId = oglBuilder->buildProgram(g_fragShaderText);

   // Step 6: Enable the fragment shader program, and all needed textures
   //
   glUseProgram(g_programId);
   glUniform1i(glGetUniformLocation(g_programId, "tex1"), 1);  // image texture
   oglBuilder->useAllTextures(g_programId);                    // LUT textures


<!-- tabs:start -->

#### **C++**


GpuShaderDesc

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static GpuShaderDescRcPtr GpuShaderDesc::CreateLegacyShaderDesc(unsigned edgelen)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Create the legacy shader description</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static GpuShaderDescRcPtr GpuShaderDesc::CreateShaderDesc()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Create the default shader description</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void GpuShaderDesc::setLanguage(GpuLanguage lang)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Set the shader program language</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>GpuLanguage GpuShaderDesc::getLanguage() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void GpuShaderDesc::setFunctionName(const char * name)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Set the function name of the shader program</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * GpuShaderDesc::getFunctionName() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void GpuShaderDesc::setPixelName(const char * name)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Set the pixel name variable holding the color values</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * GpuShaderDesc::getPixelName() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void GpuShaderDesc::setResourcePrefix(const char * prefix)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Set a prefix to the resource name      ?> **Note:**     Some applications require that textures, uniforms,      and helper methods be uniquely named because several      processor instances could coexist.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * GpuShaderDesc::getResourcePrefix() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual const char * GpuShaderDesc::getCacheID() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual unsigned GpuShaderDesc::getNumUniforms() const = 0</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Dynamic Property related methods.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual unsigned GpuShaderDesc::getTextureMaxWidth() const = 0</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>1D lut related methods</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual unsigned GpuShaderDesc::getNum3DTextures() const = 0</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>3D lut related methods</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual void GpuShaderDesc::addToDeclareShaderCode(const char * shaderCode) = 0</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Methods to specialize parts of a OCIO shader program      **An OCIO shader program could contain:**      1. A declaration part  e.g., uniform sampled3D tex3;      2. Some helper methods      3. The OCIO shader function may be broken down as:         1. The function header  e.g., void OCIODisplay(in vec4 inColor) {      2. The function body    e.g.,   vec4 outColor.rgb = texture3D(tex3, inColor.rgb).rgb;      3. The function footer  e.g.,   return outColor; }         **Usage Example:**      Below is a code snippet to highlight the different parts of the OCIO shader program.      .. code-block:: cpp            // All global declarations      uniform sampled3D tex3;         // All helper methods      vec3 computePosition(vec3 color)      {         vec3 coords = color;         // Some processing...         return coords;      }         // The shader function      vec4 OCIODisplay(in vec4 inColor)     //      {                                     // Function Header         vec4 outColor = inColor;           //            outColor.rgb = texture3D(tex3, computePosition(inColor.rgb)).rgb;            return outColor;                   // Function Footer      }                                     //</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual void GpuShaderDesc::createShaderText( const char * shaderDeclarations, const char * shaderHelperMethods, const char * shaderFunctionHeader, const char * shaderFunctionBody, const char * shaderFunctionFooter) = 0</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Create the OCIO shader program      ?> **Note:**        The OCIO shader program is decomposed to allow a specific implementation     to change some parts. Some product integrations add the color processing     within a client shader program, imposing constraints requiring this flexibility.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual const char * GpuShaderDesc::getShaderText() const = 0</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Get the complete OCIO shader program</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual void GpuShaderDesc::finalize() = 0</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>GpuShaderDesc::GpuShaderDesc()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>virtual GpuShaderDesc::~GpuShaderDesc()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<!-- tabs:end -->

## Context

<!-- tabs:start -->

#### **C++**


Context

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static ContextRcPtr Context::Create()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>ContextRcPtr Context::createEditableCopy() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Context::getCacheID() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Context::setSearchPath(const char * path)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Context::getSearchPath() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Context::getNumSearchPaths() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Context::getSearchPath(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Context::clearSearchPaths()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Context::addSearchPath(const char * path)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Context::setWorkingDir(const char * dirname)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Context::getWorkingDir() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Context::setStringVar(const char * name, const char * value)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Context::getStringVar(const char * name) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>int Context::getNumStringVars() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Context::getStringVarNameByIndex(int index) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Context::clearStringVars()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Context::setEnvironmentMode(EnvironmentMode mode)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>EnvironmentMode Context::getEnvironmentMode() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Context::loadEnvironment()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Seed all string vars with the current environment.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Context::resolveStringVar(const char * val) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Do a file lookup.      Evaluate the specified variable (as needed). Will not throw exceptions.</td>
</tr>
</table>
<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char * Context::resolveFileLocation(const char * filename) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Do a file lookup.      Evaluate all variables (as needed).   Also, walk the full search path until the file is found.   If the filename cannot be found, an exception will be thrown.</td>
</tr>
</table>
<!-- tabs:end -->
