include Makefile.arch
SrcSuf = cc

CXX  += -std=c++1z -fdiagnostics-color=always -Wall
LIBS += -lboost_system -lboost_filesystem

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

TRAJMEAS_HISTOGRAM_FILLER_MODULE_S = ./src/TrajMeasHistogramFillerModule.$(SrcSuf)
TRAJMEAS_HISTOGRAM_FILLER_MODULE_O = ./obj/TrajMeasHistogramFillerModule.$(ObjSuf)
OBJS     += $(TRAJMEAS_HISTOGRAM_FILLER_MODULE_O)

FILTERCALIBRATION_MODULE_S = ./src/FilterCalibrationModule.$(SrcSuf)
FILTERCALIBRATION_MODULE_O = ./obj/FilterCalibrationModule.$(ObjSuf)
OBJS     += $(FILTERCALIBRATION_MODULE_O)

COSMIC_TRAJMEAS_HISTOS_MODULE_S = ./src/CosmicTrajMeasHistosModule.$(SrcSuf)
COSMIC_TRAJMEAS_HISTOS_MODULE_O = ./obj/CosmicTrajMeasHistosModule.$(ObjSuf)
OBJS     += $(COSMIC_TRAJMEAS_HISTOS_MODULE_O)

CLUSTEROCCUPANCY_MODULE_S = ./src/ClusterOccupancyModule.$(SrcSuf)
CLUSTEROCCUPANCY_MODULE_O = ./obj/ClusterOccupancyModule.$(ObjSuf)
OBJS     += $(CLUSTEROCCUPANCY_MODULE_O)

# PROGRAMS

EFFICIENCY_MAIN_S = ./src/efficiency_main.$(SrcSuf)
EFFICIENCY_MAIN_O = ./obj/efficiency_main.$(ObjSuf)
EFFICIENCY_MAIN_A = ./bin/efficiency_main$(ExeSuf)
OBJS     += $(EFFICIENCY_MAIN_O)
PROGRAMS += $(EFFICIENCY_MAIN_A)

EFFICIENCY_MULTIHITMEAS_S = ./src/efficiency_multiHitMeas.$(SrcSuf)
EFFICIENCY_MULTIHITMEAS_O = ./obj/efficiency_multiHitMeas.$(ObjSuf)
EFFICIENCY_MULTIHITMEAS_A = ./bin/efficiency_multiHitMeas$(ExeSuf)
OBJS     += $(EFFICIENCY_MULTIHITMEAS_O)
PROGRAMS += $(EFFICIENCY_MULTIHITMEAS_A)

EFFICIENCY_FILTERCALIBRATION_S = ./src/efficiency_filterCalibration.$(SrcSuf)
EFFICIENCY_FILTERCALIBRATION_O = ./obj/efficiency_filterCalibration.$(ObjSuf)
EFFICIENCY_FILTERCALIBRATION_A = ./bin/efficiency_filterCalibration$(ExeSuf)
OBJS     += $(EFFICIENCY_FILTERCALIBRATION_O)
PROGRAMS += $(EFFICIENCY_FILTERCALIBRATION_A)

COSMIC_MUON_PLOTTER_S = ./src/cosmicMuonPlotter.$(SrcSuf)
COSMIC_MUON_PLOTTER_O = ./obj/cosmicMuonPlotter.$(ObjSuf)
COSMIC_MUON_PLOTTER_A = ./bin/cosmicMuonPlotter$(ExeSuf)
OBJS     += $(COSMIC_MUON_PLOTTER_O)
PROGRAMS += $(COSMIC_MUON_PLOTTER_A)

MERGE_SIMHIT_PLOTS_S = ./src/mergeSimhitPlots.$(SrcSuf)
MERGE_SIMHIT_PLOTS_O = ./obj/mergeSimhitPlots.$(ObjSuf)
MERGE_SIMHIT_PLOTS_A = ./bin/mergeSimhitPlots$(ExeSuf)
OBJS     += $(MERGE_SIMHIT_PLOTS_O)
PROGRAMS += $(MERGE_SIMHIT_PLOTS_A)

DISK1_PROPAGATION_CHECK_S = ./src/disk1PropagationCheck.$(SrcSuf)
DISK1_PROPAGATION_CHECK_O = ./obj/disk1PropagationCheck.$(ObjSuf)
DISK1_PROPAGATION_CHECK_A = ./bin/disk1PropagationCheck$(ExeSuf)
OBJS     += $(DISK1_PROPAGATION_CHECK_O)
PROGRAMS += $(DISK1_PROPAGATION_CHECK_A) 

all: $(PROGRAMS)

# Executables

$(EFFICIENCY_MAIN_A): $(EFFICIENCY_MAIN_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O)
	@printf "Compiling done, linking efficiency_main...\n"
	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

$(EFFICIENCY_MULTIHITMEAS_A): $(EFFICIENCY_MULTIHITMEAS_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O)
	@printf "Compiling done, linking efficiency_multiHitMeas...\n"
	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

$(EFFICIENCY_FILTERCALIBRATION_A): $(EFFICIENCY_FILTERCALIBRATION_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O) $(CLUSTEROCCUPANCY_MODULE_O) $(TRAJMEAS_HISTOGRAM_FILLER_MODULE_O) $(FILTERCALIBRATION_MODULE_O)
	@printf "Compiling done, linking efficiency_filterCalibration...\n"
	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

$(COSMIC_MUON_PLOTTER_A): $(COSMIC_MUON_PLOTTER_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O) $(CLUSTEROCCUPANCY_MODULE_O) $(TRAJMEAS_HISTOGRAM_FILLER_MODULE_O) $(COSMIC_TRAJMEAS_HISTOS_MODULE_O)
	@printf "Compiling done, linking cosmicMuonPlotter...\n"
	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

$(MERGE_SIMHIT_PLOTS_A): $(MERGE_SIMHIT_PLOTS_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O) $(CLUSTEROCCUPANCY_MODULE_O) $(TRAJMEAS_HISTOGRAM_FILLER_MODULE_O) $(FILTERCALIBRATION_MODULE_O)
	@printf "Compiling done, linking mergeSimhitPlots...\n"
	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

$(DISK1_PROPAGATION_CHECK_A): $(DISK1_PROPAGATION_CHECK_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O) $(CLUSTEROCCUPANCY_MODULE_O) $(TRAJMEAS_HISTOGRAM_FILLER_MODULE_O) $(FILTERCALIBRATION_MODULE_O)
	@printf "Compiling done, linking disk1PropagationCheck...\n"
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

$(TRAJMEAS_HISTOGRAM_FILLER_MODULE_O): $(TRAJMEAS_HISTOGRAM_FILLER_MODULE_S)
	@printf "Compiling utility: \"TrajMeasHistogramFillerModule\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(FILTERCALIBRATION_MODULE_O): $(FILTERCALIBRATION_MODULE_S)
	@printf "Compiling utility: \"FilterCalibrationModule\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(COSMIC_TRAJMEAS_HISTOS_MODULE_O): $(COSMIC_TRAJMEAS_HISTOS_MODULE_S)
	@printf "Compiling utility: \"CosmicTrajMeasHistosModule\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(CLUSTEROCCUPANCY_MODULE_O): $(CLUSTEROCCUPANCY_MODULE_S)
	@printf "Compiling utility: \"ClusterOccupancyModule\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

###

$(EFFICIENCY_MAIN_O): $(EFFICIENCY_MAIN_S)  
	@printf "Compiling program: \"efficiency_main\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(EFFICIENCY_MULTIHITMEAS_O): $(EFFICIENCY_MULTIHITMEAS_S)  
	@printf "Compiling program: \"efficiency_multiHitMeas\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(EFFICIENCY_FILTERCALIBRATION_O): $(EFFICIENCY_FILTERCALIBRATION_S)
	@printf "Compiling program: \"efficiency_filterCalibration\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(COSMIC_MUON_PLOTTER_O): $(COSMIC_MUON_PLOTTER_S)
	@printf "Compiling program: \"cosmicMuonPlotter\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(MERGE_SIMHIT_PLOTS_O): $(MERGE_SIMHIT_PLOTS_S)
	@printf "Compiling program: \"mergeSimhitPlots\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(DISK1_PROPAGATION_CHECK_O): $(DISK1_PROPAGATION_CHECK_S)
	@printf "Compiling program: \"disk1PropagationCheck\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

latex: LATEX_WORKING_DIRECTORY="EfficiencyPresentation"_`date "+%Y_%m_%d%n"`
latex:
	@echo "Project directory: "$(LATEX_WORKING_DIRECTORY)
	@rm $(LATEX_WORKING_DIRECTORY) -rf
	@mkdir $(LATEX_WORKING_DIRECTORY)
	@cp Latex_templates/EfficiencyCutsPresentation/* $(LATEX_WORKING_DIRECTORY) -r
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && ln -s ../../../results/*.eps .
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits._used_to_calculate_efficiency,_layers_1-4,_disks_1-3_[with_cuts].eps to pdf..." && epstopdf num._hits._used_to_calculate_efficiency,_layers_1-4,_disks_1-3_[with_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_layers_1-4,_disks_1-3_[with_cuts].eps to pdf..." && epstopdf efficiency,_layers_1-4,_disks_1-3_[with_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy,_phi_vs_z_-_forward.eps to pdf..." && epstopdf cluster_occupancy,_phi_vs_z_-_forward.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy,_phi_vs_z_-_forward.eps to pdf..." && epstopdf rechit_occupancy,_phi_vs_z_-_forward.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits_used_to_calc._eff.,_phi_vs_z_-_forward.eps to pdf..." && epstopdf num._hits_used_to_calc._eff.,_phi_vs_z_-_forward.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_phi_vs_z_-_forward.eps to pdf..." && epstopdf efficiency,_phi_vs_z_-_forward.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy,_phi_vs_z_-_layer_1.eps to pdf..." && epstopdf cluster_occupancy,_phi_vs_z_-_layer_1.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy,_phi_vs_z_-_layer_1.eps to pdf..." && epstopdf rechit_occupancy,_phi_vs_z_-_layer_1.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_1.eps to pdf..." && epstopdf num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_1.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_phi_vs_z_-_layer_1.eps to pdf..." && epstopdf efficiency,_phi_vs_z_-_layer_1.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy,_phi_vs_z_-_layer_2.eps to pdf..." && epstopdf cluster_occupancy,_phi_vs_z_-_layer_2.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy,_phi_vs_z_-_layer_2.eps to pdf..." && epstopdf rechit_occupancy,_phi_vs_z_-_layer_2.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_2.eps to pdf..." && epstopdf num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_2.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_phi_vs_z_-_layer_2.eps to pdf..." && epstopdf efficiency,_phi_vs_z_-_layer_2.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy,_phi_vs_z_-_layer_3.eps to pdf..." && epstopdf cluster_occupancy,_phi_vs_z_-_layer_3.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy,_phi_vs_z_-_layer_3.eps to pdf..." && epstopdf rechit_occupancy,_phi_vs_z_-_layer_3.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_3.eps to pdf..." && epstopdf num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_3.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_phi_vs_z_-_layer_3.eps to pdf..." && epstopdf efficiency,_phi_vs_z_-_layer_3.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy,_phi_vs_z_-_layer_4.eps to pdf..." && epstopdf cluster_occupancy,_phi_vs_z_-_layer_4.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy,_phi_vs_z_-_layer_4.eps to pdf..." && epstopdf rechit_occupancy,_phi_vs_z_-_layer_4.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_4.eps to pdf..." && epstopdf num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_4.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_phi_vs_z_-_layer_4.eps to pdf..." && epstopdf efficiency,_phi_vs_z_-_layer_4.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_[no_cuts].eps to pdf..." && epstopdf lx_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_[with_other_effcuts].eps to pdf..." && epstopdf lx_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_[no_cuts].eps to pdf..." && epstopdf ly_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_[with_other_effcuts].eps to pdf..." && epstopdf ly_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_fwd._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_fwd._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_fwd._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_fwd._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_fwd._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_fwd._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_l1._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_l1._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_l2._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_l2._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_l3._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_l3._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_l4._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_l4._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_fwd._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_fwd._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_fwd._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_fwd._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_fwd._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_fwd._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_l1._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_l1._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_l2._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_l2._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_l3._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_l3._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_l4._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_l4._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_inner_l1._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_inner_l1._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_inner_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_inner_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_inner_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_inner_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_outer_l1._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_outer_l1._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_outer_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_outer_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_outer_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_outer_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_inner_l2._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_inner_l2._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_inner_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_inner_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_inner_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_inner_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_outer_l2._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_outer_l2._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_outer_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_outer_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_outer_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_outer_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_inner_l3._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_inner_l3._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_inner_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_inner_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_inner_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_inner_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_outer_l3._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_outer_l3._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_outer_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_outer_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_outer_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_outer_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_inner_l4._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_inner_l4._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_inner_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_inner_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_inner_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_inner_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_outer_l4._modules_[no_cuts].eps to pdf..." && epstopdf lx_on_outer_l4._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting lx_on_outer_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf lx_on_outer_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_lx_on_outer_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_lx_on_outer_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_inner_l1._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_inner_l1._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_inner_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_inner_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_inner_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_inner_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_outer_l1._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_outer_l1._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_outer_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_outer_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_outer_l1._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_outer_l1._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_inner_l2._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_inner_l2._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_inner_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_inner_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_inner_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_inner_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_outer_l2._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_outer_l2._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_outer_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_outer_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_outer_l2._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_outer_l2._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_inner_l3._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_inner_l3._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_inner_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_inner_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_inner_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_inner_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_outer_l3._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_outer_l3._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_outer_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_outer_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_outer_l3._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_outer_l3._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_inner_l4._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_inner_l4._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_inner_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_inner_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_inner_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_inner_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_outer_l4._modules_[no_cuts].eps to pdf..." && epstopdf ly_on_outer_l4._modules_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting ly_on_outer_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf ly_on_outer_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_ly_on_outer_l4._modules_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_ly_on_outer_l4._modules_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_lay1_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_lay1_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_lay1_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_lay1_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_lay1_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_lay1_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_lay2_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_lay2_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_lay2_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_lay2_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_lay2_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_lay2_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_lay3_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_lay3_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_lay3_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_lay3_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_lay3_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_lay3_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_lay4_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_lay4_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_lay4_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_lay4_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_lay4_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_lay4_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._1,_inner_mods._[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._1,_inner_mods._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._1,_inner_mods._[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._1,_inner_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_pos._lay._1,_inner_mods._[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_pos._lay._1,_inner_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._2,_inner_mods._[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._2,_inner_mods._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._2,_inner_mods._[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._2,_inner_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_pos._lay._2,_inner_mods._[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_pos._lay._2,_inner_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._3,_inner_mods._[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._3,_inner_mods._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._3,_inner_mods._[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._3,_inner_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_pos._lay._3,_inner_mods._[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_pos._lay._3,_inner_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._4,_inner_mods._[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._4,_inner_mods._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._4,_inner_mods._[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._4,_inner_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_pos._lay._4,_inner_mods._[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_pos._lay._4,_inner_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._1,_outer_mods._[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._1,_outer_mods._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._1,_outer_mods._[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._1,_outer_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_pos._lay._1,_outer_mods._[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_pos._lay._1,_outer_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._2,_outer_mods._[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._2,_outer_mods._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._2,_outer_mods._[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._2,_outer_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_pos._lay._2,_outer_mods._[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_pos._lay._2,_outer_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._3,_outer_mods._[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._3,_outer_mods._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._3,_outer_mods._[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._3,_outer_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_pos._lay._3,_outer_mods._[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_pos._lay._3,_outer_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._4,_outer_mods._[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._4,_outer_mods._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_pos._lay._4,_outer_mods._[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_pos._lay._4,_outer_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_pos._lay._4,_outer_mods._[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_pos._lay._4,_outer_mods._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_negative_z,_inner_ring,_panel_2_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_negative_z,_inner_ring,_panel_2_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_negative_z,_inner_ring,_panel_2_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_negative_z,_inner_ring,_panel_2_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_-_fwd,_negative_z,_inner_ring,_panel_2_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_-_fwd,_negative_z,_inner_ring,_panel_2_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_1_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_1_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_1_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_1_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_1_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_1_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_2_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_2_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_2_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_2_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_2_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_-_fwd,_negative_z,_outer_ring,_panel_2_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_1_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_1_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_1_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_1_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_1_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_1_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_2_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_2_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_2_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_2_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_2_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_-_fwd,_positive_z,_inner_ring,_panel_2_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_1_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_1_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_1_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_1_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_1_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_1_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_2_[no_cuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_2_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._of_hits_vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_2_[with_other_effcuts].eps to pdf..." && epstopdf num._of_hits_vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_2_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_2_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_hit_position_-_fwd,_positive_z,_outer_ring,_panel_2_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting d0_of_barrel_hits_[no_cuts].eps to pdf..." && epstopdf d0_of_barrel_hits_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting d0_of_barrel_hits_[with_other_effcuts].eps to pdf..." && epstopdf d0_of_barrel_hits_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_d0_of_barrel_hits_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_d0_of_barrel_hits_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting d0_of_forward_hits_[no_cuts].eps to pdf..." && epstopdf d0_of_forward_hits_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting d0_of_forward_hits_[with_other_effcuts].eps to pdf..." && epstopdf d0_of_forward_hits_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_d0_of_forward_hits_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_d0_of_forward_hits_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting dz_of_barrel_hits_[no_cuts].eps to pdf..." && epstopdf dz_of_barrel_hits_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting dz_of_barrel_hits_[with_other_effcuts].eps to pdf..." && epstopdf dz_of_barrel_hits_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_dz_of_barrel_hits_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_dz_of_barrel_hits_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting dz_of_forward_hits_[no_cuts].eps to pdf..." && epstopdf dz_of_forward_hits_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting dz_of_forward_hits_[with_other_effcuts].eps to pdf..." && epstopdf dz_of_forward_hits_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_dz_of_forward_hits_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_dz_of_forward_hits_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting strip_hits_[no_cuts].eps to pdf..." && epstopdf strip_hits_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting strip_hits_[with_other_effcuts].eps to pdf..." && epstopdf strip_hits_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_n._strip_hits_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_n._strip_hits_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting parent_vertex_n._tracks_[no_cuts].eps to pdf..." && epstopdf parent_vertex_n._tracks_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting parent_vertex_n._tracks_[with_other_effcuts].eps to pdf..." && epstopdf parent_vertex_n._tracks_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_parent_vertex_n._tracks_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_parent_vertex_n._tracks_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting closest_cluster_distance_to_traj._meas._[no_cuts].eps to pdf..." && epstopdf closest_cluster_distance_to_traj._meas._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting closest_cluster_distance_to_traj._meas._[with_other_effcuts].eps to pdf..." && epstopdf closest_cluster_distance_to_traj._meas._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_closest_cluster_distance_to_traj._meas._[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_closest_cluster_distance_to_traj._meas._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting closest_hit_distance_to_traj._meas._[no_cuts].eps to pdf..." && epstopdf closest_hit_distance_to_traj._meas._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting closest_hit_distance_to_traj._meas._[with_other_effcuts].eps to pdf..." && epstopdf closest_hit_distance_to_traj._meas._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_closest_hit_distance_to_traj._meas._[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_closest_hit_distance_to_traj._meas._[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting pt_[no_cuts].eps to pdf..." && epstopdf pt_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting pt_[with_other_effcuts].eps to pdf..." && epstopdf pt_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting eff._vs_pt_[with_other_effcuts].eps to pdf..." && epstopdf eff._vs_pt_[with_other_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Cleaning up eps soft links..." && rm *.eps
	@cd $(LATEX_WORKING_DIRECTORY) && pdflatex main.tex # needs to be done at least twice :S
	@cd $(LATEX_WORKING_DIRECTORY) && pdflatex main.tex

latex_cosmics: LATEX_WORKING_DIRECTORY="CosmicsStatsPresentation"_`date "+%Y_%m_%d%n"`
latex_cosmics:
	@echo "Project directory: "$(LATEX_WORKING_DIRECTORY)
	@rm $(LATEX_WORKING_DIRECTORY) -rf
	@mkdir $(LATEX_WORKING_DIRECTORY)
	@cp Latex_templates/CosmicsPresentation/* $(LATEX_WORKING_DIRECTORY) -r
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && ln -s ../../../results/*.eps .
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting closest_cluster_distance_to_traj._meas.,_barrel_[no_cuts].eps to pdf..." && epstopdf closest_cluster_distance_to_traj._meas.,_barrel_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting closest_cluster_distance_to_traj._meas.,_forward_[no_cuts].eps to pdf..." && epstopdf closest_cluster_distance_to_traj._meas.,_forward_[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting closest_cluster_distance_to_traj._meas._[no_cuts].eps to pdf..." && epstopdf closest_cluster_distance_to_traj._meas._[no_cuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy,_phi_vs_z_-_forward.eps to pdf..." && epstopdf cluster_occupancy,_phi_vs_z_-_forward.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy,_phi_vs_z_-_forward.eps to pdf..." && epstopdf rechit_occupancy,_phi_vs_z_-_forward.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits_used_to_calc._eff.,_phi_vs_z_-_forward.eps to pdf..." && epstopdf num._hits_used_to_calc._eff.,_phi_vs_z_-_forward.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_phi_vs_z_-_forward.eps to pdf..." && epstopdf efficiency,_phi_vs_z_-_forward.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy,_phi_vs_z_-_layer_1.eps to pdf..." && epstopdf cluster_occupancy,_phi_vs_z_-_layer_1.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy,_phi_vs_z_-_layer_1.eps to pdf..." && epstopdf rechit_occupancy,_phi_vs_z_-_layer_1.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_1.eps to pdf..." && epstopdf num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_1.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_phi_vs_z_-_layer_1.eps to pdf..." && epstopdf efficiency,_phi_vs_z_-_layer_1.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy,_phi_vs_z_-_layer_2.eps to pdf..." && epstopdf cluster_occupancy,_phi_vs_z_-_layer_2.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy,_phi_vs_z_-_layer_2.eps to pdf..." && epstopdf rechit_occupancy,_phi_vs_z_-_layer_2.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_2.eps to pdf..." && epstopdf num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_2.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_phi_vs_z_-_layer_2.eps to pdf..." && epstopdf efficiency,_phi_vs_z_-_layer_2.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy,_phi_vs_z_-_layer_3.eps to pdf..." && epstopdf cluster_occupancy,_phi_vs_z_-_layer_3.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy,_phi_vs_z_-_layer_3.eps to pdf..." && epstopdf rechit_occupancy,_phi_vs_z_-_layer_3.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_3.eps to pdf..." && epstopdf num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_3.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_phi_vs_z_-_layer_3.eps to pdf..." && epstopdf efficiency,_phi_vs_z_-_layer_3.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy,_phi_vs_z_-_layer_4.eps to pdf..." && epstopdf cluster_occupancy,_phi_vs_z_-_layer_4.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy,_phi_vs_z_-_layer_4.eps to pdf..." && epstopdf rechit_occupancy,_phi_vs_z_-_layer_4.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_4.eps to pdf..." && epstopdf num._hits_used_to_calc._eff.,_phi_vs_z_-_layer_4.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting efficiency,_phi_vs_z_-_layer_4.eps to pdf..." && epstopdf efficiency,_phi_vs_z_-_layer_4.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy_on_the_ROCs_-_forward.eps to pdf..." && epstopdf cluster_occupancy_on_the_ROCs_-_forward.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy_forward.eps to pdf..." && epstopdf rechit_occupancy_forward.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._tracks_used_to_calc._eff._fwd._[with_effcuts].eps to pdf..." && epstopdf num._tracks_used_to_calc._eff._fwd._[with_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting roc_efficiency_forward_[with_effcuts].eps to pdf..." && epstopdf roc_efficiency_forward_[with_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy_on_the_ROCs_-_layer_1.eps to pdf..." && epstopdf cluster_occupancy_on_the_ROCs_-_layer_1.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy_layer_1.eps to pdf..." && epstopdf rechit_occupancy_layer_1.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._tracks_used_to_calc._eff._l1._[with_effcuts].eps to pdf..." && epstopdf num._tracks_used_to_calc._eff._l1._[with_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting roc_efficiency_layer_1_[with_effcuts].eps to pdf..." && epstopdf roc_efficiency_layer_1_[with_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy_on_the_ROCs_-_layer_2.eps to pdf..." && epstopdf cluster_occupancy_on_the_ROCs_-_layer_2.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy_layer_2.eps to pdf..." && epstopdf rechit_occupancy_layer_2.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._tracks_used_to_calc._eff._l2._[with_effcuts].eps to pdf..." && epstopdf num._tracks_used_to_calc._eff._l2._[with_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting roc_efficiency_layer_2_[with_effcuts].eps to pdf..." && epstopdf roc_efficiency_layer_2_[with_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy_on_the_ROCs_-_layer_3.eps to pdf..." && epstopdf cluster_occupancy_on_the_ROCs_-_layer_3.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy_layer_3.eps to pdf..." && epstopdf rechit_occupancy_layer_3.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._tracks_used_to_calc._eff._l3._[with_effcuts].eps to pdf..." && epstopdf num._tracks_used_to_calc._eff._l3._[with_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting roc_efficiency_layer_3_[with_effcuts].eps to pdf..." && epstopdf roc_efficiency_layer_3_[with_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting cluster_occupancy_on_the_ROCs_-_layer_4.eps to pdf..." && epstopdf cluster_occupancy_on_the_ROCs_-_layer_4.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting rechit_occupancy_layer_4.eps to pdf..." && epstopdf rechit_occupancy_layer_4.eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting num._tracks_used_to_calc._eff._l4._[with_effcuts].eps to pdf..." && epstopdf num._tracks_used_to_calc._eff._l4._[with_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Converting roc_efficiency_layer_4_[with_effcuts].eps to pdf..." && epstopdf roc_efficiency_layer_4_[with_effcuts].eps
	@cd $(LATEX_WORKING_DIRECTORY)/Images/Plots && echo "Cleaning up eps soft links..." && rm *.eps
	@cd $(LATEX_WORKING_DIRECTORY) && pdflatex main.tex # needs to be done at least twice :S
	@cd $(LATEX_WORKING_DIRECTORY) && pdflatex main.tex

clean:
	@rm -f $(OBJS) $(PROGRAMS) core

distclean: clean
	@rm -f $(PROGRAMS) *Dict.* *.def *.exp \
	*.root *.ps *.so *.lib *.dll *.d *.log .def so_locations
	@rm -rf cxx_repository

.SUFFIXES: .$(SrcSuf)
