inc_dir = include/
src_dir = src/
obj_dir = obj/
CCOMPILER  = g++
DEBUG = -O2 #-ggdb  #-g -gstabs -gstabs+ -ggdb -glevel

object = $(obj_dir)match.o \
		$(obj_dir)LHAASOMatch.o $(obj_dir)WFCTASLC.o \
		$(obj_dir)EventInfo.o $(obj_dir)LedEvtSel.o \
		$(obj_dir)EventDict.o \
		$(obj_dir)WFCTAEvent.o $(obj_dir)TAEvent.o


match: $(obj_dir)match.a
	cp $(src_dir)EventDict_rdict.pcm ./
	$(CCOMPILER) -o match $(obj_dir)match.a `root-config --cflags --libs` -lMinuit -lMatrix

$(obj_dir)match.a: $(object)
	ar -r $(obj_dir)match.a $(object)

$(obj_dir)match.o: match.cc Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir) `root-config --cflags --libs`

$(obj_dir)LHAASOMatch.o: $(src_dir)LHAASOMatch.cpp Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir)

$(obj_dir)WFCTASLC.o: $(src_dir)WFCTASLC.cpp Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir) `root-config --cflags --libs`

$(obj_dir)EventInfo.o: $(src_dir)EventInfo.cpp Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir) `root-config --cflags --libs`

$(obj_dir)LedEvtSel.o: $(src_dir)LedEvtSel.cpp Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir) `root-config --cflags --libs`

$(obj_dir)EventDict.o: $(src_dir)EventDict.cpp Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir) `root-config --cflags --libs`

$(obj_dir)WFCTAEvent.o: $(src_dir)WFCTAEvent.cpp Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir) `root-config --cflags --libs`

$(obj_dir)TAEvent.o: $(src_dir)TAEvent.cpp Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir) `root-config --cflags --libs`

.PHONY : clean
 clean :
	rm match EventDict_rdict.pcm obj/*
