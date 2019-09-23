This is a test




dfsdf

[](ocio_colorspace.md ':include')

## Config

A config defines all the color spaces to be available at runtime.

The OCIO color configuration (Config) is the main object for interacting with this library. It encapsulates all of the information necessary to use customized ColorSpaceTransform and DisplayTransform operations.

See the [User Guide](overview/configs.md) for more information on selecting, creating, and working with custom color configurations.

For applications interested in using only one color config at a time (this is the vast majority of apps), their API would traditionally get the global configuration and use that, as opposed to creating a new one. This simplifies the use case for plugins and bindings, as it alleviates the need to pass around configuration handles.

An example of an application where this would not be sufficient would be a multi-threaded image proxy server (daemon), which wished to handle multiple show configurations in a single process concurrently. This app would need to keep multiple configurations alive, and to manage them appropriately.

Roughly speaking, a novice user should select a default configuration that most closely approximates the use case (animation, visual effects, etc.), and set the OCIO environment variable to point at the root of that configuration.

?> **Note:** Initialization using environment variables is typically preferable in a multi-app ecosystem, as it allows all applications to be consistently configured.

ConstConfigRcPtr GetCurrentConfig()
Get the current configuration.

void SetCurrentConfig(const ConstConfigRcPtr& config)
Set the current configuration. This will then store a copy of the specified config.

### Initialization

<!-- tabs:start -->

#### **C++**

Class `OpenColorIO::Config`

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
    <td><code>static ConfigRcPtr Config::CreateFromEnv()</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Constructor a configuration using the OCIO environmnet variable.</td>
</tr>
</table>

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static ConfigRcPtr Config::CreateFromStream(std::istream& istream)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Constructor a configuration using a specific config file.</td>
</tr>
</table>

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>static ConfigRcPtr Config::CreateFromFile(const char* filename)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Constructor a configuration using a specific config file.</td>
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


### Utilities

<!-- tabs:start -->

#### **C++**

Class `OpenColorIO::Config`

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::sanityCheck() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>This will throw an exception if the config is malformed. The most common error occurs when references are made to colorspaces that do not exist.</td>
</tr>
</table>

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char* Config::getDescription() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::setDescription(const char* description)</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>void Config::serialize(std::ostream& os) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>Returns the string representation of the Config in YAML text form. This is typically stored on disk in a file with the extension .ocio.</td>
</tr>
</table>

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char* Config::getCacheID() const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td>This will produce a hash of the all colorspace definitions, etc. All external references, such as files used in FileTransforms, etc., will be incorporated into the cacheID.While the contents of the files are not read, the file system is queried for relavent information (mtime, inode) so that the config’s cacheID will change when the underlying luts are updated. If a context is not provided, the current Context will be used. If a null context is provided, file references will not be taken into account (this is essentially a hash of Config::serialize).</td>
</tr>
</table>

<table>
<tr>
    <th scope="row">Function</th>
    <td><code>const char* Config::getCacheID(const ConstContextRcPtr& context) const</code></td>
</tr>
<tr>
    <th scope="row">Description</th>
    <td></td>
</tr>
</table>

#### **Python**

<!-- tabs:end -->

Resources
Given a lut src name, where should we find it?



ConstContextRcPtr Config::getCurrentContext() const
void Config::addEnvironmentVar(const char* name, const char* defaultValue)
int Config::getNumEnvironmentVars() const
const char* Config::getEnvironmentVarNameByIndex(int index) const
const char* Config::getEnvironmentVarDefault(const char* name) const
void Config::clearEnvironmentVars()
void Config::setEnvironmentMode(EnvironmentMode mode)
EnvironmentMode Config::getEnvironmentMode() const
void Config::loadEnvironment()
const char* Config::getSearchPath() const
void Config::setSearchPath(const char* path)
const char* Config::getWorkingDir() const
void Config::setWorkingDir(const char* dirname)
ColorSpaces
int Config::getNumColorSpaces() const
const char* Config::getColorSpaceNameByIndex(int index) const
This will null if an invalid index is specified

Note

These fcns all accept either a color space OR role name. (Colorspace names take precedence over roles.)

ConstColorSpaceRcPtr Config::getColorSpace(const char* name) const
This will return null if the specified name is not found.

int Config::getIndexForColorSpace(const char* name) const
void Config::addColorSpace(const ConstColorSpaceRcPtr& cs)
Note

If another color space is already registered with the same name, this will overwrite it. This stores a copy of the specified color space.

void Config::clearColorSpaces()
const char* Config::parseColorSpaceFromString(const char* str) const
Given the specified string, get the longest, right-most, colorspace substring that appears.

If strict parsing is enabled, and no color space is found, return an empty string.
If strict parsing is disabled, return ROLE_DEFAULT (if defined).
If the default role is not defined, return an empty string.
bool Config::isStrictParsingEnabled() const
void Config::setStrictParsingEnabled(bool enabled)
Roles
A role is like an alias for a colorspace. You can query the colorspace corresponding to a role using the normal getColorSpace fcn.

void Config::setRole(const char* role, const char* colorSpaceName)
Note

Setting the colorSpaceName name to a null string unsets it.

int Config::getNumRoles() const
bool Config::hasRole(const char* role) const
Return true if the role has been defined.

const char* Config::getRoleName(int index) const
Get the role name at index, this will return values like ‘scene_linear’, ‘compositing_log’. Return empty string if index is out of range.

Display/View Registration
Looks is a potentially comma (or colon) delimited list of lookNames, Where +/- prefixes are optionally allowed to denote forward/inverse look specification. (And forward is assumed in the absence of either)

const char* Config::getDefaultDisplay() const
int Config::getNumDisplays() const
const char* Config::getDisplay(int index) const
const char* Config::getDefaultView(const char* display) const
int Config::getNumViews(const char* display) const
const char* Config::getView(const char* display, int index) const
const char* Config::getDisplayColorSpaceName(const char* display, const char* view) const
const char* Config::getDisplayLooks(const char* display, const char* view) const
void Config::addDisplay(const char* display, const char* view, const char* colorSpaceName, const char* looks)
For the (display,view) combination, specify which colorSpace and look to use. If a look is not desired, then just pass an empty string

void Config::clearDisplays()
void Config::setActiveDisplays(const char* displays)
Comma-delimited list of display names.

const char* Config::getActiveDisplays() const
void Config::setActiveViews(const char* views)
Comma-delimited list of view names.

const char* Config::getActiveViews() const
Luma
Get the default coefficients for computing luma.

Note

There is no “1 size fits all” set of luma coefficients. (The values are typically different for each colorspace, and the application of them may be nonsensical depending on the intensity coding anyways). Thus, the ‘right’ answer is to make these functions on the Config class. However, it’s often useful to have a config-wide default so here it is. We will add the colorspace specific luma call if/when another client is interesting in using it.

void Config::getDefaultLumaCoefs(float* rgb) const
void Config::setDefaultLumaCoefs(const float* rgb)
These should be normalized (sum to 1.0 exactly).

Look
Manager per-shot look settings.

ConstLookRcPtr Config::getLook(const char* name) const
int Config::getNumLooks() const
const char* Config::getLookNameByIndex(int index) const
void Config::addLook(const ConstLookRcPtr& look)
void Config::clearLooks()
Processors
Convert from inputColorSpace to outputColorSpace

Note

This may provide higher fidelity than anticipated due to internal optimizations. For example, if the inputcolorspace and the outputColorSpace are members of the same family, no conversion will be applied, even though strictly speaking quantization should be added.

If you wish to test these calls for quantization characteristics, apply in two steps; the image must contain RGB triples (though arbitrary numbers of additional channels can be supported (ignored) using the pixelStrideBytes arg).

ConstProcessorRcPtr Config::getProcessor(const ConstContextRcPtr& context, const ConstColorSpaceRcPtr& srcColorSpace, const ConstColorSpaceRcPtr& dstColorSpace) const
ConstProcessorRcPtr Config::getProcessor(const ConstColorSpaceRcPtr& srcColorSpace, const ConstColorSpaceRcPtr& dstColorSpace) const
ConstProcessorRcPtr Config::getProcessor(const char* srcName, const char* dstName) const
Note

Names can be colorspace name, role name, or a combination of both.

ConstProcessorRcPtr Config::getProcessor(const ConstContextRcPtr& context, const char* srcName, const char* dstName) const
Get the processor for the specified transform.

Not often needed, but will allow for the re-use of atomic OCIO functionality (such as to apply an individual LUT file).

ConstProcessorRcPtr Config::getProcessor(const ConstTransformRcPtr& transform) const
ConstProcessorRcPtr Config::getProcessor(const ConstTransformRcPtr& transform, TransformDirection direction) const
ConstProcessorRcPtr Config::getProcessor(const ConstContextRcPtr& context, const ConstTransformRcPtr& transform, TransformDirection direction) const

[](ocio_globals.md ':include')