# Benjamin Benoit's Format for Project Metadata

## Specification

| Specification |                                                                         |
| ------------- | ----------------------------------------------------------------------- |
| Codification  | ASCII (American Standard Code for Information Interchange)              |
| Structure     | key-value structure with deterministic keys and arbitrary values        |
| Keys          | NAME, REPOSITORY, VERSION, RELEASE-DATE, AUTHORS, DEPENDENCIES, LICENSE |
| Values        | Arbitrary                                                               |
| Commentaries  | Converts the line into a commentary with the '#' character.             |

## Syntax

The BBFPM is key-value format easy, minimalistic and deterministic that defines a list of seven deterministic keys with arbitrary values. What does this mean? Every key MUST be defined in an specific order, but the values depends on the user who is defining those keys. The format looks like this:

```bbfpm
# Benjamin Benoit's Format for Project Metadata
NAME: "[PROJECT NAME]"
REPOSITORY: "[URL]"
VERSION: "[VERSION]"
RELEASE-DATE: "[RELEASE-DATE]"
AUTHORS: ["[AUTHOR1]"@"[EMAIL]", "[AUTHOR2]"@"[EMAIL]"]
DEPENDENCIES: ["[DEPENDENCY]"@"[VERSION]", "[DEPENDENCY2]"@"[VERSION]"]
LICENSE: "GPL-3.0-or-later"
```

| Keys           |                                                                                                                                                               |
| -------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `NAME`         | **Project's name**. Arbitrary value. `NONE` NOT ALLOWED.                                                                                                      |
| `REPOSITORY`   | **Project's repository URL**. Arbitrary value. `NONE` ALLOWED.                                                                                                |
| `VERSION`      | **Project's version**. Arbitrary value. `NONE` ALLOWED.                                                                                                       |
| `RELEASE-DATE` | **Project's last release date**. Arbitrary value. `NONE` Allowed.                                                                                             |
| `AUTHORS`      | **Project's list of authors** co-related arbitrary values (preferable arbitrary values: real **NAME** and **EMAIL**). `NONE` NOT ALLOWED.                     |
| `DEPENDENCIES` | **Project's list of dependencies** co-related arbitrary values (preferable arbitrary values: **DEPENDENCY NAME** and **DEPENDENCY VERSION**). `NONE` ALLOWED. |
| `LICENSE`      | **Project's license**. `NONE` ALLOWED.                                                                                                                        |

### Restrictions:

- Literal quation marks are NOT ALLOWED inside of arbitrary values.
- Empty lists are NOT ALLOWED. Instead, use `NONE` keyword.
