//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "hello"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

STATISTIC(HelloCounter, "Counts number of functions greeted");

namespace {
	
	//Structure to hold stats about a function
	struct functionStats{
		int basicBlocks;
		int CFG;
		int singleLoop;
		int basicBlockLoop;
		int dominators;

		//Constructor
		functionStats(){
			basicBlocks = 0;
			CFG= 0;
			singleLoop= 0;
			basicBlockLoop = 0;
			dominators = 0;
		}
	};

	//Structure to hold individual stats about module
	struct individualStats{
		int min;
		int max;
		int total;
		int num;
		
		//Constructor
		individualStats(){
			min = 0;
			max = 0;
			total = 0;
			num = 0;
		}
		//Add new information
		void add(int newStat){
			//Comapre to min/max
			if (newStat>max){
				max = newStat;
			}else if(newStat<min){
				min = newStat;
			}
			//Add new info
			total+=newStat;
			num++;
		}
		//Print information
		void print(){
			errs() << "min:";
			errs() << "max:";
			errs() << "avg:";
		}

	};

	//Structure to hold stats about module
	struct moduleStats{
		individualStats basicBlocks;
		individualStats CFG;
		individualStats singleLoop;
		individualStats basicBlockLoop;
		individualStats dominators;	

		//Add other functions stats to module
		void add(functionStats newStats){
			basicBlocks.add(newStats.basicBlocks);
			CFG.add(newStats.CFG);
			singleLoop.add(newStats.singleLoop);
			basicBlockLoop.add(newStats.basicBlockLoop);
			dominators.add(newStats.dominators);	
		}
		//Print all information
		void print(){
			errs() << "Basic Blocks:";
			basicBlocks.print();

			errs() << "CFG:";
			CFG.print();

			errs() << "Single Loops:";
			singleLoop.print();

			errs() << "Basic Blocks:";
			basicBlockLoop.print();

			errs() << "Dominators:";
			dominators.print();
		}
	};
	
	struct Hello : public ModulePass {
		// Pass identification, replacement for typeid
		static char ID; 
		Hello() : ModulePass(ID) {}

		//Initialize moduleStats
		moduleStats fullStats;

		virtual bool runOnModule(Module &M) {
			++HelloCounter;
			errs() << "asd:Hello: ";
			//errs().write_escaped(F.getName()) << '\n';
			return false;
		}
	};
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass");

