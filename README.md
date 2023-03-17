# A glibc Collation Compatibility Library

* [Overview](#overview)
* [Getting started](#getting-started)
* [Help & feedback](#help--feedback)
* [Contributing](#contributing)
* [Security](#security)
* [License](#license)

## Overview

glibc is the GNU C Library implementation, which is used on all major Linux distributions (e.g. CentOS/AlmaLinux/Rocky, Debian/Ubuntu, SuSE). The glibc library, libc.so, provides most of the foundational C routines such as open, read, write, malloc, printf, and literally thousands more. It also provides the interface to the Linux kernel via syscalls. For the purposes of this discussion, the facility of interest is the locale functionality, and more specifically the functions that provide string sorting according to localized collation rules.

Locale specific sorting is important and relevant for programs such as PostgreSQL. That is because, as a database, PostgreSQL must frequently sort and then persist string data according to the specified locale collation. In order for this to work durably and correctly, the sort order must be determinant and immutable.

Therein lies the problem at hand. Since glibc implements the sort order, if/when glibc changes the sort order from one version to the next, it breaks the contract with PostgreSQL, and thereby causes data corruption. Indexes that have been persisted to storage may now memorialize the data in the wrong order according to the currently installed version of glibc.

On a given major version of a Linux distribution, say CentOS, the major version of glibc is supposed to be maintained stable. Therefore, changes to collation theoretically should not happen within one distribution. But it can occur due to inadvertent/unintentional collation changes during a minor version update to glibc. Even if the effects of this event are limited to a small number of users, the impact on those users is significant.

Moreover, when an existing database is moved from one Linux distribution major version to another (say CentOS 7 to CentOS 8), the major version of glibc typically changes (in this example from 2.17 to 2.28), bringing with it potentially widespread significant changes in collations and sort order. For anyone, but especially for large database fleets, this is a huge problem when the need to migrate Linux distribution for other reasons presents itself.

In summary, if a PostgreSQL database resides on an CentOS7 system with glibc version 2.17, and the operating system (OS) is upgraded to CentOS8 with glibc version 2.28, the majority of indexes built on collatable columns will be broken.

## Getting started

This branch `2.26-59.amzn2` is for glibc version 2.26-59 included with Amazon Linux 2.

* SOURCES folder contains original sources which are used by rpmbuild for building the collation compatibility library rpm.
* SPEC folder has the glibc.spec
* devel folder has extracted source code (rpmbuild -bp ...)
* glibc-compatcollation.sh script to help with various tasks related to development and packaging

### Tags

* 2.26-59.amzn2-BASELINE - original source from glibc-2.26-59.amzn2.src.rpm
* 2.26-59.amzn2-v1.5 - Release 1.5 for collation compatibility library

### For Package Builder
 
#### Building RPM

* Build with default options

`./glibc-compatcollation.sh build`

* Set extra args for rpmbuild

`RPMBUILD_EXTRAS="--define 'compatprefix /opt'" ./glibc-compatcollation.sh build`

### For patch developers

Patch development steps as follows:

1. Setup build environment

`./glibc-compatcollation.sh build`

2. Update code in devel/ directory or SPECS/glibc.spec

3. Build the library

`./glibc-compatcollation.sh rebuild`

4. Test libraries

5. Generate patch files

`./glibc-compatcollation.sh rpmpatch`

6. Commit changes

7. Build RPM

`./glibc-compatcollation.sh build`

8. Test RPM

9. Commit and TAG

10. Push upstream

### Show code changes

* Show git diff since last release (includes uncommitted changes)

`./glibc-compatcollation.sh diff`

* Show git diff between last release and given TAG or COMMMIT

`./glibc-compatcollation.sh diff <TAG|COMMIT>`

Example:

`./glibc-compatcollation.sh diff HEAD`

| This will show all the committed changes since last release

* Show git diff between two TAGS or COMMITS

`./glibc-compatcollation.sh diff <TAG1|COMMIT1> <TAG2|COMMIT2>`

Example:

`./glibc-compatcollation.sh diff 2.26-59.amzn2-BASELINE 2.26-59.amzn2-v1.5`

`./glibc-compatcollation.sh diff 2.26-59.amzn2-v1.5 2.26-59.amzn2-v1.7`

## Help & feedback

Have a question? Have a feature request? We recommend trying the following things (in this order):

* Read README.md
* [Search open issues](https://github.com/awslabs/compat-collation-for-glibc/issues)
* [Open a new issue](https://github.com/awslabs/compat-collation-for-glibc/issues/new)

## Contributing

We welcome and encourage contributions!

See our [contribution guide](CONTRIBUTING.md) for more information on how to report issues, set up a development environment, and submit code.

We adhere to the [Amazon Open Source Code of Conduct](https://aws.github.io/code-of-conduct).

## Security

See [CONTRIBUTING](CONTRIBUTING.md#security-issue-notifications) for more information.

## License

See [LICENSE](LICENSE.md) for license information.


