# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project **only** adheres to the following _(as defined at [Semantic Versioning](https://semver.org/spec/v2.0.0.html))_:

> Given a version number MAJOR.MINOR.PATCH, increment the:
> 
> - MAJOR version when you make incompatible API changes
> - MINOR version when you add functionality in a backward compatible manner
> - PATCH version when you make backward compatible bug fixes

## [5.0.2] - 2026-XX-XX

This release restores execution of the except and finally PEPs, fixes how `Query.total_rows()` handles recounting, and improves safety for rule execution. It also adds new options to the build hook.

### Changed

- Update Python interpreter version requirement in README (#53).
- Mirror the iRODS Rule Language rule engine plugin's clearing of the rError stack for dynamic PEPs (#272).

### Fixed

- Fix `Query.total_rows()` recount for case-insensitive queries (#258).
- Use raw string literal in regex to avoid syntax warning (#261).
- Update default dynamic PEP regex to cover except and finally PEPs (#263).
- Do not allow other plugins to invoke `register_regexes_from_array` (#267).
- Extend safety of Python rule execution (#269).

### Added

- Add build hook option for compiling in debug mode (#265).
- Add build hook option for compiling against specific version of released iRODS packages (irods/irods_development_environment#165).
