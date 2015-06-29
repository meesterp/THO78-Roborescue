include Makefile.inc

.PHONY : mission1 localisationmodule lidar rosbee environmentsimulator test clean

echoUIHeaders:
	@echo UI is: $(UI)
	@echo UI headers are: $(UI_HEADERS)
	
generateUIHeaders: $(UI)
	@echo UI is: $(UI)
	@echo UI headers are: $(UI_HEADERS)
	
echoUIHeaders:
	
mission1 : 

	@echo UI is: $(UI)
	@echo UI headers are: $(UI_HEADERS)

	$(UI_HEADERS) $(OBJ)
		$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)
	
localisationmodule : $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)
	
lidar : $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)
	
rosbee : 
	
environmentsimulator : $(UI_HEADERS) $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

test : $(UI_HEADERS) $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)
	
clean : 
	for prefix in $(MODULES); do \
		rm -f "$$prefix"*.o; \
		rm -f "$$prefix"*.d; \
		rm -f "$$prefix"ui_*.h; \
		rm -f "$$prefix"moc_*.cpp; \
	done
	rm -f *.exe

-include $(OBJ:.o=.d)
