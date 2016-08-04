import shutil
import os
import zipfile
import distutils.dir_util
import distutils.file_util
import sys
from subprocess import call

def zipdir(path, zip):
    for root, dirs, files in os.walk(path):
        for file in files:
            zip.write(os.path.join(root, file))


if len(sys.argv) < 2:
    print "To few arguments"
    exit


    
project_dir = sys.argv[1]

path_to_res = sys.argv[2]
path_to_exe = project_dir + 'RelWithDebInfo/'
destination = project_dir + '../Deploy/AzureAssault/';
deploy_path = project_dir + '../Deploy/';


path_to_data = project_dir + 'ProjectData/'
path_to_projectinfo = path_to_data + "/ProjectInfo.txt"

with open(path_to_projectinfo) as f:
    project_info = f.readlines()
project_info = [line.rstrip('\n') for line in project_info]

project_name = project_info[0]
project_version = project_info[1]


    

if not os.path.exists(destination):
    os.makedirs(destination)

file = 'fmod.dll'
distutils.file_util.copy_file(path_to_exe + file, destination + file)

file = 'ProjectCrimson.exe'
dest_file = 'AzureAssault.exe'
distutils.file_util.copy_file(path_to_exe + file, destination + dest_file)

file = 'readme.txt'
distutils.file_util.copy_file(path_to_res + file, destination + file)

file = 'Res/'
distutils.dir_util.copy_tree(path_to_res + file, destination + file)

file = 'VS/CrashRpt/bin'
distutils.dir_util.copy_tree(path_to_res + file, destination)



#zip deploy
zip_path = deploy_path + project_name + " (" + project_version + ").zip";
zipf = zipfile.ZipFile(zip_path, 'w')
zipdir(destination, zipf)
zipf.close()

#zip PDB
zip_path = deploy_path + project_name + " (" + project_version + ") PDB.zip";
zipf = zipfile.ZipFile(zip_path, 'w')
zipf.write(path_to_exe + "projectcrimson.pdb")
zipf.close()


#send files to doctor dump
guid = "e54d2648-d95a-49ff-ac83-1961295440f6"
file = 'VS/CrashRpt/SymUpload.exe'
symupload_path = path_to_res + file
args = "{} {}.0 0 \"{}\"".format(guid, project_version, destination + 'AzureAssault.exe')
print "Calling symupload with args " + symupload_path + " " + args
args = [symupload_path, guid, project_version+".0", "0", destination + 'AzureAssault.exe'];
call(args)
args[4] = path_to_exe + "projectcrimson.pdb"
call(args)
