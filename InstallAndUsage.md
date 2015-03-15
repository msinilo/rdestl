How to make best use of RDE stl in your projects

# Introduction #
  * Checkout/Export the source from SVN
  * Add the local copy to your IDE or Makefile's Include search path
  * To include all RDE STL classes at once, `#include <rdestl.h>`
  * Add `RDE_DEBUG` to your debug configuration's preprocessor defines

Please note, to use `rde::string`, you must `#include <rde_string.h>`  (i.e. **not** `#include <string.h>` - this is to avoid confusion for you _and_ the compiler)

# Running Unit Tests (MSVC++ 2008) #
  * Get [UnitTest++](http://unittest-cpp.sourceforge.net)
  * Add it to VC's include search path
  * Open Test/Build/Win32/stltest.sln
  * if needed, remove and re-add the UnitTest++.vc2005.vcproj to your solution (no doubt your local copy of UnitTest++ isn't in the same location as mine)
  * Build & Run