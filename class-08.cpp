//
//  class-08.cpp
//  proj08
//
//  Created by Bryan J Kars on 11/2/14.
//  Copyright (c) 2014 Bryan J Kars. All rights reserved.
//

#include "class-08.h"
#include <cstdlib>
using std::atol;
using std::stol;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::istringstream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
using std::pair;
using std::make_pair;
#include <algorithm>
using std::transform;
using std::copy;
using std::begin;
using std::end;
using std::to_string;
#include<iostream>
using std::cout;
using std::endl;
#include <random>



Program::Program(string file_name){
    
    string instruction; //holds the line in the .txt file
    ifstream myfile(file_name);// file to open
    
    if (myfile.is_open()){

        while(getline(myfile, instruction)){ //loops through the lines in specified .txt
            
            //cout << instruction<< endl;
            instructions_.push_back(instruction); //pushback into vector
            
        }
        
        instruction_index_ = 0; //set bases
        set_status("running"); //set status
        
        myfile.close();
    }
}

string Program::get_instruction(bool lock_flag){
    
    if(instruction_index_ == this->instructions_.size()){
        return ""; //return empty string if index is equal to amount of instructions
    }
    else if(lock_flag == true){ // return waiting if program is locked
        return "waiting";
    }
    else{
        
       
        string string_return = instructions_[instruction_index_];
        //cout << string_return << endl;
        instruction_index_+=1; //move instruction up 1 and return the current instruction or i guess the last one
        return string_return;
    }
}

CPU::CPU(initializer_list<Program> p_list){
    for( auto itr = p_list.begin(); itr != p_list.end(); itr++ ){
        programs_.push_back(*itr); //iterate through the list and add them to programs_ vector
    }
    lock_flag_ = false; //set all the data members defaults
    program_index_ = 0;
    memory_.clear();
    current_instruction_ = "";
    instruction_fields_.clear();
    
}

bool CPU::finished(){
    
    for(int i = 0; i < programs_.size(); i++){ // iterates throught the status' of programs and returns weather or not they are running or finished
        
        if(programs_[i].get_status() == "running"){
            return false;
        }
    }
    return true;
}

void CPU::run(){
    
    string lock0 = "unlocked";
    string lock1 = "unlocked";
    
    while(!finished()){ //as long as they are still running
        //loop through the programs_ vector and run the specified functions for each
        
        split_instruction();
        execute_instruction();
        
        program_index_ += 1;
        program_index_ = program_index_ % programs_.size();
        instruction_fields_.clear();
        
    }
}

bool CPU::is_assignment(){ //checks for the assignment symbol in the second item in instruction_fields_, if so TRUE
    if(instruction_fields_[1] == "="){
        return true;
    }
    return false;
}

bool CPU::is_print(){ //checks for the string print within the first item in vector instruction_fields_, if so TRUE
    if(instruction_fields_[0] == "print"){
        return true;
    }
    return false;
}

bool CPU::is_end(){ //checks for the string end in the first item in the vector instruction_fields_, if so TRUE
    if(instruction_fields_[0] == "end"){
        return true;
    }
    return false;
}

bool CPU::is_lock(){ //checks for the string lock in the first item of vector
    if(instruction_fields_[0] == "lock"){
        return true;
    }
    return false;
}

bool CPU::is_unlock(){ //checks for string unlock in first item in the vector
    if(instruction_fields_[0] == "unlock"){
        return true;
    }
    return false;
}

void CPU::split_instruction(){
    // breaks the string current_instruction into peices through istringstream and places them into the instruction feilds vector
    current_instruction_ = programs_[program_index_].get_instruction(lock_flag_);
    istringstream iss (current_instruction_);
    string temp;
    
    while(iss >> temp){

        instruction_fields_.push_back(temp);
    }
    
}
void CPU::execute_instruction(){
    //checks to see what the current instruction is then does the appropriate thing
    if(is_assignment()){
        
        string var = instruction_fields_[0];
        long val = stol(instruction_fields_[2]);
        memory_[var] = val; //adds the val and var to the memory map and displays for debugging
        //cout << "ass" << var << "=" << val << endl;
        
    }
    else if(is_print()){
        
        auto temp = instruction_fields_[1]; // checks map of assigned values and when found prints them out
        cout << memory_[temp] << endl;
    }
    else if(is_end()){
         // if the command is end sets the programs status to finished
        programs_[program_index_].set_status("finished");
        
    }
    /*
    else if(is_lock()){ //changes the lock value to true if it is beinf enabled
        lock_flag_ = true;
    }
    else if(is_unlock()){
        lock_flag_ = false;// changes the lock value to false if it is unlocking
    }
     */
}