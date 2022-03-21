# rdestl

[rdestl](https://github.com/msinilo/rdestl) is a C++ library by [Maciej Sinilo](https://github.com/msinilo) providing a subset of STL functionality aimed at game development. The library has a strong focus on highly-optimized code without the use of exceptions or RTTI.

Related blog posts: http://msinilo.pl/blog/?cat=6

## Usage

How to make best use of RDE stl in your projects:

- Clone rdestl to your project's working folder, or a directory you have added to your include paths.

	```bash
	$ cd MyProject
	$ git clone https://github.com/msinilo/rdestl
	```

- To include all rdestl classes at once, simply add `#include <rdestl.h>` (this can be added to your `pch.h` or `stdafx.h` if using a precompiled headers).

	> **Note:** If using `rde::string`, be sure to use `#include <rde_string.h>` (*not* `#include <string.h>` or `#include <string>`). This is to avoid confusion for both you *and* the compiler.

- Add `RDE_DEBUG` to your preprocessor defines for your Debug configuration.

## Unit Tests

Tests are included in the `Test/` directory, and depend on the [Catch](https://github.com/catchorg/Catch2/tree/Catch1.x) framework (included as a single-file header at `Test/vendor/Catch/catch.hpp`)

To run tests, simply open `rdestl/Test/rdestltest.sln`, build, and run.
