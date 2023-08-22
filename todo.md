# VENTURI / OAK TODO LIST
-------------------------
## OAK

- [x] 1. update panel management with a Panel Manager class, change vector to Map, template the panel fucntions
- [ ] 2. project management system
- [ ] 3. user preference struct
- [x] 4. update window control to better mimic native windows control (snapping, resize, etc)
- [x] 5. update loggint to add trace for better filtering and diagnostics
- [x] 6. add AppConsole log sink
- [x] 7. restructure folder layout

---------------------------
## VENTURI

- [ ] 1. add style management system
- [ ] 2. 

### PLOTS
- [x] 1. add a list of active DataSeries, use DataManager->GetSeries(id) to retrieve the data pointers for plotting

----------------------------
## WI

### data series
- [x] 1. remove inheritance from DataSeries class - this class will now just hold the data and a few ubiquitous methods
- [x] 2. define interfaces for loading, writing, and processing DataSeries'. 
- [x] 3. implement the interfaces for an Oscsillator type data source
- [x] 4. remove source type from DataSeries - this isn't really useful, and then simplifes the map of DataSeries so we can loop over it in the application code
- [ ] 5. 

### loading

- osciilator series are generated on the fly
- streaming series are read from TCP (or UDP?) data stream
- file series should either load the full file for viewing/manipulating the data, or stream at a predefined playback rate (probably should load the whole file anyway)


### processing
- define an ordered data struct of proc funcs that are applied to the data in an ordered pipeline
- need the ability to push, pop, emplace, and remove funcs
- if we want to define these in a graphical way, we may need to recompile the stack after any changes
