#!/usr/bin/env python

import os, sys, shutil, tempfile, subprocess, re, argparse
from datetime import date

# def replace(source_file_path, pattern, substring):
# 	fh, target_file_path = tempfile.mkstemp()
# 	with open(target_file_path, 'w') as target_file:
# 		with open(source_file_path, 'r') as source_file:
# 			for line in source_file:
# 				target_file.write(line.replace(pattern, substring))
# 	os.remove(source_file_path)
# 	shutil.move(target_file_path, source_file_path)

def main():
	# Parse command line arguments
	parser = argparse.ArgumentParser()
	parser.add_argument('-r', '--run_number', default = "")
	parser.add_argument('-w', '--wbc_label', default = "")
	# parser.add_argument('-v', dest='verbose', action='store_true')
	args = parser.parse_args()
	# 
	print "%s started..." % sys.argv[0]
	today = date.today()
	latex_working_directory = "CosmicsStatsPresentation_%s" % today.strftime("%Y_%m_%d")
	latex_main = latex_working_directory + "/main.tex"
	print "Project working directory: %s" % latex_working_directory
	print "Run number set to: %s" % args.run_number
	print "WBC label set to: %s" % args.wbc_label
	shutil.rmtree(latex_working_directory, ignore_errors = True)
	shutil.copytree("Latex_templates/CosmicsPresentation", latex_working_directory)
	print "%s terminated succesfully." % sys.argv[0]
	plot_base_names = [
		"associated_clust._Y_distance,_barrel_+_fwd",
		"associated_clust._Y_distance,_barrel_only",
		"associated_clust._Y_distance,_forward_only",
		"associated_clust._X_distance,_barrel_+_fwd",
		"associated_clust._X_distance,_barrel_only",
		"associated_clust._X_distance,_forward_only",
		"avg._cluster_X_distance_on_barrel_rings",
		"avg._cluster_Y_distance_on_barrel_rings",
		"cluster_occupancy,_Y_vs_X_-_barrel",
		"cluster_occupancy,_Y_vs_X_-_forward,_disk_1",
		"cluster_occupancy,_Y_vs_X_-_forward,_disk_2",
		"cluster_occupancy,_Y_vs_X_-_forward,_disk_3",
		"cluster_occupancy,_Y_vs_X_-_negative_z,_forward,_disk_1",
		"cluster_occupancy,_Y_vs_X_-_negative_z,_forward,_disk_2",
		"cluster_occupancy,_Y_vs_X_-_negative_z,_forward,_disk_3",
		"cluster_occupancy,_Y_vs_X_-_positive_z,_forward,_disk_1",
		"cluster_occupancy,_Y_vs_X_-_positive_z,_forward,_disk_2",
		"cluster_occupancy,_Y_vs_X_-_positive_z,_forward,_disk_3",
		"cluster_occupancy,_phi_vs_z_-_forward",
		"cluster_occupancy,_phi_vs_z_-_layer_1",
		"cluster_occupancy,_phi_vs_z_-_layer_2",
		"cluster_occupancy,_phi_vs_z_-_layer_3",
		"cluster_occupancy,_phi_vs_z_-_layer_4",
		"cluster_occupancy_on_the_ROCs_-_forward",
		"cluster_occupancy_on_the_ROCs_-_layer_1",
		"cluster_occupancy_on_the_ROCs_-_layer_2",
		"cluster_occupancy_on_the_ROCs_-_layer_3",
		"cluster_occupancy_on_the_ROCs_-_layer_4",
		"cluster_within_5_mm_(for_each_direction)",
		"efficiency,_phi_vs_z_-_forward",
		"efficiency,_phi_vs_z_-_layer_1",
		"efficiency,_phi_vs_z_-_layer_2",
		"efficiency,_phi_vs_z_-_layer_3",
		"efficiency,_phi_vs_z_-_layer_4",
		"num._rechits_on_barrel_rings",
		"rechit_occupancy,_Y_vs_X_-_barrel",
		"rechit_occupancy,_Y_vs_X_-_forward,_disk_1",
		"rechit_occupancy,_Y_vs_X_-_forward,_disk_2",
		"rechit_occupancy,_Y_vs_X_-_forward,_disk_3",
		"rechit_occupancy,_Y_vs_X_-_negative_z,_forward,_disk_1",
		"rechit_occupancy,_Y_vs_X_-_negative_z,_forward,_disk_2",
		"rechit_occupancy,_Y_vs_X_-_negative_z,_forward,_disk_3",
		"rechit_occupancy,_Y_vs_X_-_positive_z,_forward,_disk_1",
		"rechit_occupancy,_Y_vs_X_-_positive_z,_forward,_disk_2",
		"rechit_occupancy,_Y_vs_X_-_positive_z,_forward,_disk_3",
		"rechit_occupancy,_phi_vs_z_-_forward",
		"rechit_occupancy,_phi_vs_z_-_layer_1",
		"rechit_occupancy,_phi_vs_z_-_layer_2",
		"rechit_occupancy,_phi_vs_z_-_layer_3",
		"rechit_occupancy,_phi_vs_z_-_layer_4",
		"rechit_occupancy_forward",
		"rechit_occupancy_layer_1",
		"rechit_occupancy_layer_2",
		"rechit_occupancy_layer_3",
		"rechit_occupancy_layer_4",
		"rechit_pixel_clusters,_cluster_within_5_mm",
		"rechits,_cluster_within_1cm",
		"roc_efficiency_forward_[with_effcuts]",
		"roc_efficiency_layer_1_[with_effcuts]",
		"roc_efficiency_layer_2_[with_effcuts]",
		"roc_efficiency_layer_3_[with_effcuts]",
		"roc_efficiency_layer_4_[with_effcuts]"]
	for i in range(len(plot_base_names)):
		modified_plot_name = "RUN:_%s_%s:_" % (args.run_number, args.wbc_label) + plot_base_names[i]
		command = "epstopdf \"results/%s.eps\" -outfile=\"%s/Images/Plots/%s.pdf\"" % (modified_plot_name, latex_working_directory, modified_plot_name)
		# print(command)
		print "Converting %s.eps to pdf..." % modified_plot_name
		subprocess.call(command, shell = True)
	for i, plot_base_name in enumerate(plot_base_names):
		characters_to_escape = ['[', ']', '.']
		for character in characters_to_escape:
			plot_base_name = plot_base_name.replace(character, "\\%s" % character)
		modified_plot_name = "RUN:_%s_%s:_" % (args.run_number, args.wbc_label) + plot_base_name
		command = "sed -i \"s;%s;%s;\" %s" % (plot_base_name, modified_plot_name, latex_main)
		# print command
		subprocess.call(command, shell = True)
	command = "cd %s && pdflatex main.tex" % latex_working_directory
	subprocess.call(command, shell = True) # needs to be done at least twice :S
	subprocess.call(command, shell = True)

if __name__ == "__main__":
	main()


