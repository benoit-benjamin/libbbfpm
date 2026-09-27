# Benjamin Benoit's Format for Project Metadata

## Data structures

`BBFPM__Metadata` opaque struct that contains the metadata from
the file provided:

```c
typedef struct BBFPM__Metadata BBFPM__Metadata;
```

---

`BBFPM__CorrelatedArbitraryValue` struct that contains an individual
arbitrary value correlation:

```c
typedef struct BBFPM__CorrelatedArbitraryValue
{
    char* value;
    char* correlation;
} BBFPM__CorrelatedArbitraryValue;
```

---

`BBFPM__CorrelatedArbitraryValueList` struct that contains a list of
individual arbitrary value correlation:

```c
typedef struct BBFPM__CorrelatedArbitraryValueList
{
    BBFPM__CorrelatedArbitraryValue* correlations;
    uint32_t                         correlations__total;
} BBFPM__CorrelatedArbitraryValueList;
```

---

## Functions

### Lifecycle

| Function                                                                                | Description                                                                 |
| --------------------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| `BBFPM__LoadMetadata(const char* filepath)`                                             | Loads metadata from file. Returns `NULL` on failure.                        |
| `BBFPM__DestroyMetadata(BBFPM__Metadata* metadata)`                                     | Frees all memory owned by the metadata.                                     |
| `BBFPM__DestroyCorrelatedArbitraryValueList(BBFPM__CorrelatedArbitraryValueList* list)` | Frees a list of individual corelated arbitrary values returned by a getter. |

---

### Getters — Arbitrary Values

Return `char*`. **Do not free.** Returns `NULL` in case the metadata entry is `NONE`:

| Function                        | Field          |
| ------------------------------- | -------------- |
| `BBFPM__GetMetadataName`        | `NAME`         |
| `BBFPM__GetMetadataRepository`  | `REPOSITORY`   |
| `BBFPM__GetMetadataVersion`     | `VERSION`      |
| `BBFPM__GetMetadataReleaseDate` | `RELEASE-DATE` |
| `BBFPM__GetMetadataLicense`     | `LICENSE`      |

---

### Getters — Correlated Lists

Return `BBFPM__CorrelatedArbitraryValueList` by value. Free with `BBFPM__DestroyCorrelatedArbitraryValueList`.

| Function                         | Field          |
| -------------------------------- | -------------- |
| `BBFPM__GetMetadataAuthors`      | `AUTHORS`      |
| `BBFPM__GetMetadataDependencies` | `DEPENDENCIES` |

---

### Examples

```c
#include <stdio.h>
#include <bbfpm/bbfpm.h>

// Load the metadata and store it into the BBFPM__Metadata struct
BBFPM__Metadata* metadata = BBFPM__LoadMetadata("./metadata.bbfpm");
if (metadata == NULL) { /* { ... } */}

// Get metadata value of type arbitrary value
printf(
    "NAME:         %s\n"
    "REPOSITORY:   %s\n"
    "VERSION:      %s\n"
    "RELEASE-DATE: %s\n"
    "LICENSE:      %s\n",
    BBFPM__GetMetadataName(metadata),
    BBFPM__GetMetadataRepository(metadata),
    BBFPM__GetMetadataVersion(metadata),
    BBFPM__GetMetadataReleaseDate(metadata),
    BBFPM__GetMetadataLicense(metadata)
);

// Destroy metadata loaded
BBFPM__DestroyMetadata(metadata);

```

### List of correlated arbitrary values

```c
#include <stdio.h>
#include <bbfpm/bbfpm.h>

// Load the metadata and store it into the BBFPM__Metadata struct
BBFPM__Metadata* metadata = BBFPM__LoadMetadata("./metadata.bbfpm");
if (metadata == NULL) { /* { ... } */}

// Get metadata value of type list of correlated arbitrary values
BBFPM__CorrelatedArbitraryValueList metadata__authors = BBFPM__GetMetadataAuthors(metadata);
BBFPM__CorrelatedArbitraryValueList metadata__dependencies = BBFPM__GetMetadataDependencies(metadata);

// Print authors
printf("AUTHORS:\n");
for (int i = 0; i < metadata__authors.correlations__total; i++) {
    BBFPM__CorrelatedArbitraryValue correlated_arbitrary_value = metadata__authors.correlations[i];
    printf("    %s - %s\n", correlated_arbitrary_value.value, correlated_arbitrary_value.correlation);
}

// Print dependencies
printf("DEPENDENCIES:\n");
for (int i = 0; i < metadata__dependencies.correlations__total; i++) {
    BBFPM__CorrelatedArbitraryValue correlated_arbitrary_value = metadata__dependencies.correlations[i];
    printf("    %s - %s\n", correlated_arbitrary_value.value, correlated_arbitrary_value.correlation);
}

// Destroy the lists of correlated arbitrary values
BBFPM__DestroyCorrelatedArbitraryValueList(&metadata__authors);
BBFPM__DestroyCorrelatedArbitraryValueList(&metadata__dependencies);

// Destroy metadata loaded
BBFPM__DestroyMetadata(metadata);
```
