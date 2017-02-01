include Makefile.arch
SrcSuf = cc

CXX += -std=c++14 -fdiagnostics-color=always -Wall

# Utility

CONSOLECOLORS_S = ./src/ConsoleColors.$(SrcSuf)
CONSOLECOLORS_O = ./obj/ConsoleColors.$(ObjSuf)
OBJS     += $(CONSOLECOLORS_O)

CONSOLEACTOR_S = ./src/ConsoleActor.$(SrcSuf)
CONSOLEACTOR_O = ./obj/ConsoleActor.$(ObjSuf)
OBJS     += $(CONSOLEACTOR_O)

COMMONACTORS_S = ./src/CommonActors.$(SrcSuf)
COMMONACTORS_O = ./obj/CommonActors.$(ObjSuf)
OBJS     += $(COMMONACTORS_O)

TIMER_S = ./src/Timer.$(SrcSuf)
TIMER_O = ./obj/Timer.$(ObjSuf)
OBJS     += $(TIMER_O)

TIMERCOL_S = ./src/TimerColored.$(SrcSuf)
TIMERCOL_O = ./obj/TimerColored.$(ObjSuf)
OBJS     += $(TIMERCOL_O)

TTREETOOLS_S = ./src/TTreeTools.$(SrcSuf)
TTREETOOLS_O = ./obj/TTreeTools.$(ObjSuf)
OBJS     += $(TTREETOOLS_O)

CANVASEXTRAS_S = ./src/CanvasExtras.$(SrcSuf)
CANVASEXTRAS_O = ./obj/CanvasExtras.$(ObjSuf)
OBJS     += $(CANVASEXTRAS_O)

# PROGRAMS

EFFICIENCY_MAIN_S = ./src/efficiency_main.$(SrcSuf)
EFFICIENCY_MAIN_O = ./obj/efficiency_main.$(ObjSuf)
EFFICIENCY_MAIN_A = ./bin/efficiency_main$(ExeSuf)
OBJS     += $(EFFICIENCY_MAIN_O)
PROGRAMS += $(EFFICIENCY_MAIN_A)

all: $(PROGRAMS)

# Executables

$(EFFICIENCY_MAIN_A): $(EFFICIENCY_MAIN_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O)
	@printf "Compiling done, linking efficiency_main...\n"
	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

# Utility obj files

$(CONSOLECOLORS_O): $(CONSOLECOLORS_S)
	@printf "Compiling utility: \"ConsoleColors\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(CONSOLEACTOR_O): $(CONSOLEACTOR_S)
	@printf "Compiling utility: \"ConsoleActor\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(COMMONACTORS_O): $(COMMONACTORS_S)
	@printf "Compiling utility: \"CommonActors\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(TIMER_O): $(TIMER_S)
	@printf "Compiling utility: \"TimerColored\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(TIMERCOL_O): $(TIMERCOL_S)
	@printf "Compiling utility: \"TimerColored\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(TTREETOOLS_O): $(TTREETOOLS_S)
	@printf "Compiling utility: \"TTreeTools\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(CANVASEXTRAS_O): $(CANVASEXTRAS_S)
	@printf "Compiling utility: \"CanvasExtras\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

###

$(EFFICIENCY_MAIN_O): $(EFFICIENCY_MAIN_S)  
	@printf "Compiling program: \"efficiency_main\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

clean:
	@rm -f $(OBJS) $(PROGRAMS) core

distclean: clean
	@rm -f $(PROGRAMS) *Dict.* *.def *.exp \
	*.root *.ps *.so *.lib *.dll *.d *.log .def so_locations
	@rm -rf cxx_repository

.SUFFIXES: .$(SrcSuf)
