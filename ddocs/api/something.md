# Home

OpenColorIO (OCIO) is a complete color management solution geared
towards motion picture production with an emphasis on visual effects and
computer animation. OCIO provides a straightforward and consistent user
experience across all supporting applications while allowing for
sophisticated back-end configuration options suitable for high-end
production usage. OCIO is compatible with the Academy Color Encoding
Specification (ACES) and is LUT-format agnostic, supporting many popular
formats.

OpenColorIO is released as version 1.0 and has been in development since
2003. OCIO represents the culmination of years of production experience
earned on such films as SpiderMan 2 (2004), Surf's Up (2007), Cloudy
with a Chance of Meatballs (2009), Alice in Wonderland (2010), and many
more. OpenColorIO is natively supported in commercial applications like
Katana, Mari, Nuke, Silhouette FX, and others.

## Mailing Lists

There are two mailing lists associated with OpenColorIO:

  - [ocio-user](https://lists.aswf.io/g/ocio-user)`@lists.aswf.io`  
    For end users (artists, often) interested in OCIO profile design,
    facility color management, and workflow.

  - [ocio-dev](https://lists.aswf.io/g/ocio-dev)`@lists.aswf.io`  
    For developers interested OCIO APIs, code integration, compilation,
    etc.

## Quick Start

Most users will likely want to use the OpenColorIO that comes
precompiled with their applications. See the `compatiblesoftware` for
further details on each application.

Note that OCIO configurations are required to do any 'real' work, and
are available separately on the `downloads` section of this site.
Example images are also available. For assistance customizing .ocio
configurations, contact [ocio-user](https://lists.aswf.io/g/ocio-user).

  - Step 1: set the OCIO environment-variable to
    /path/to/your/profile.ocio
  - Step 2: Launch supported application.

If you are on a platform that is not envvar friendly, most applications
also provide a menu option to select a different OCIO configuration
after launch.

Please be sure to select a profile that matches your color workflow (VFX
work typically requires a different profile than animated features). If
you need assistance picking a profile, email
[ocio-user](https://lists.aswf.io/g/ocio-user).

## Downloading and Building the Code

Source code is available on Github at
<http://github.com/imageworks/OpenColorIO>

Download a
[.zip](http://github.com/imageworks/OpenColorIO/zipball/master) or
[.tar.gz](http://github.com/imageworks/OpenColorIO/tarball/master) of
the current state of the repository.

Please see the `developer-guide` for more info, and contact
[ocio-dev](https://lists.aswf.io/g/ocio-dev) with any questions.

<div class="toctree" hidden="" data-maxdepth="2">

self introduction

</div>
