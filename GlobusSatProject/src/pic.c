#include "pic.h"

//download to ground
//this has a few steps
//first lets asssume we have a pic backed up onto the sat we have to turn it into chuncks i think bc as far as ik there is a max chunck size
//this is sorta like dump so maybe i should learn beforehand?
//first in this func we can check if the file is .jpg or whatever we will use
//then we will f_tell the file if it is bigger than max chuncks we will save the first maxchunckssize into an array than transmit it with a 1
//this will make us enter a loop of saving chunck to array then transmiting til the last one 

//on the ground we will have to go through each transmition and look for missing 1.chuncks 2.chunks with missing bits(dont fit the lenght)
//ground control sends up an array and file size and the sat will check if the file is still the same size else returns E_FILE_CHANGED
//now a diff func that goes through the file chunck by chunck and check if it is in the array if so transmits

//upload to sat
//unless i come up with a better idea i can have a reverse version of what is above 
//but here i have to write a func that checks if it receved everything and send back what it finds this will have to be saved in the fram as an array
//a new cmd will have to be added GetMissingFrames() which sends back this array from the fram
//before it starts to get the pic frames makes new file name [currtime].JPG