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

In summary, if a PostgreSQL database resides on an CentOS7 system with glibc version 2.17, and the operating system (OS) is upgraded to CentOS8 with glibc version 2.28, the majority of indexes built on collatable columns will be broken. The purpose of this project is to provide a solution to that problem, in the form of portable locale library extracted from a very specific glibc version -- i.e. from a versioned package belonging to a particular Linux distribution.

## Getting started

The code in this repository is organized such that each base glibc version (the version who's collation you want to preserve) has its own branch. Once you have cloned the repository, switch to appropriate branch based on your desired base glibc verion and follow the branch specific README.md.

If a branch does not exist for your desired base glibc version, create one starting with the RPM source of that glibc package. Lay it out using the pattern in the existing branches. Then merge the difference between the latest tag and the baseline for one of the existing branches, e.g. 2.17-326.el7-v1.2 diff'ed against 2.17-326.el7-BASELINE. The details are left as an exercise for the reader.

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


