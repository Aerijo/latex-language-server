## Using the Style

1. entryName read from block
2. entryName -> Entry
3. If no Entry, we mark as unexpected and move to next entry. There is 
     little point in analysing the fields of an entry we do not know of.
4. Field names read from block. Each name used to retrieve a Field from 
     the Entry map of fieldName -> Field
5. Check one, some, and all constraints using list of field names
6. Check field constraints, resolving all values if necessary. 
     If it cannot be resolved, warn this but don't error the value as well

Some notes:
- We can't just form a single set of fields, as they can
  have different constraints based on their entry. Or rather,
  we can but I plan to tie data constraints directly to the field.
  
## Constructing a Style:

We must construct a Style iteratively, as XML does not guarantee 
the order of it's elements. Each pass cannot rely on the results
of the other steps of the pass.

1. First Pass:
    - Create map (final) of constants 
        - I'm note sure how useful this is.
        - Relevant for name fields, as the following are equivalent
        ```plain
        AUTHOR = {Hans Harman and Simon de Beumont}
        AUTHOR = {given=Hans, family=Harman and given=Simon, prefix=de, family=Beumont}
        ```
    - TODO Create whitelisted maps (temp) (store unrecognised ones separately for further processing)
    - Create map (final) of entryName -> Entry
        - Each Entry will be the "absolute" version
    - Create map (temp) of fieldName -> Field
        - We don't need this map in the final Style. These Fields are only temporary, and will be copied for each Entry
    - Create map of entryName -> vector<fieldName>
    - Create list of constraints (temp), each one holding entryNames, mandatory, conditional, and data constraints

2. Second Pass:
    - Add global data constraints to all Fields
    - Add entry constraints to each Entry
    
3. Third Pass:
    - Build fieldName -> Field map of each Entry
    
4. Fourth Pass:
    - Build pseudo entries, fields, and constraints from maps.
        - E.g., there is a map from @phdthesis{} to @thesis{type="phd"}.
            We can resolve the map at construction time by adding the 
            pseudo entry @phdthesis, with type marked as an unexpected field (or duplicated).
            The result is equivalent, and we do not need to account for the map any more
    
5. Fifth pass:
    - Consolidate entry constraints (and check for paradoxes?)
        - remove duplicates
        - remove "some" lists when an element is in "all"
        - remove "one" lists entirely from expected fields when an element is in "all" (or mark it appropriately)

## Notes

While 100% compliance is the goal, it is unlikely to be reached any time soon. 
Biber does a lot of internal manipulation in a run, but this linter needs to be 
able to make fast, iterative re-lints, and so as much work as possible is done 
in advance. In particular, handling of arbitrary maps is unlikely to be present 
for a while, as they could be difficult to handle gracefully.
