#include "pfMon.hpp"
#include "config.hpp"

#include <cstring>
#include <iostream>
#include <string>
#include <fstream>

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

void printHelp(const po::options_description& desc);
void printOgl();
void printVersion();

int selectGame(const string& game);
void doPf();

int main(int argc, char* argv[]) {
	po::options_description desc("Options");
	desc.add_options()
		("help,h,?", "Display help message")
		("ogl,o", "Display OGL text")
		("version,v", "Display version")
		("game", po::value<string>()->required(), "Select game system; this can also be the first positional argument");

	po::positional_options_description pos;
	pos.add("game", 1);

	po::variables_map vm;
	po::store(
		po::command_line_parser(argc, argv).options(desc).positional(pos).run(),
		vm);
	po::notify(vm);

	if (vm.count("help")) {
		printHelp(desc);
		return 0;
	}

	if (vm.count("version")) {
		printVersion();
		return 0;
	}

	if (vm.count("ogl")) {
		printOgl();
		return 0;
	}

	if (vm.count("game")) {
		auto game = vm["game"].as<string>();

		if (game != "pf") {
			
			return 1;
		}

		return selectGame(game);
	} else {
		cout << "Game system not specified, see --help for usage information." << endl;
		return 1;
	}

	return 0;
}

int selectGame(const string& game) {

	if (game == "pf") {
		doPf();
	} else {
		cout << "Unrecognized game system '" << game << "' -- valid systems are: pf" << endl;
		return 1;
	}

	return 0;
}

void doPf() {

	string tempStorage;


	PfMon monster;


/*****************************************************************************\
 *                                  MONSTER                                  *
\*****************************************************************************/

	
	#ifdef _DEBUG
	cout << "Monster name: " << monster.monName << endl;
	cout << "File path: " << monster.fileName << endl;
	#endif
	
	monster.CR = monster.fetchCR();
	monster.XP = monster.determineXP(monster.CR);
	monster.alignment = monster.fetchAlignment();
	monster.creatSize = monster.fetchSize();
	monster.creatType = monster.fetchCreatureType();
	monster.fetchSubtypes();
	monster.fetchAura();

	monster.fetchHd();
	monster.calculateBab();
	monster.fetchAbilities();
	monster.determineInitiative();
	monster.fetchSenses();
	monster.calculateHp();
	monster.determineAC();
	monster.determineSaves();
	monster.fetchDefensiveAbilities();
	monster.determineImmunities();
	monster.fetchResistances();
	monster.fetchWeaknesses();
	monster.fetchDr();
	monster.fetchSr();

	monster.fetchSpeed();
	monster.fetchMeleeAtk();
	monster.fetchRangedAtk();
	monster.calculateSpace();
	monster.determineReach();
	monster.fetchSpecialAttacks();

	monster.determineCombatManeuvers();
	monster.fetchFeats();
	monster.fetchSkills();
	monster.fetchLanguages();
	monster.fetchSpecialQualities();

	monster.fetchEnvironment();
	monster.fetchOrganization();
	monster.fetchTreasure();

	monster.fetchSpecialAbilities();

	#ifndef _DEBUG
	monster.prepareWrite();
	#else
	cout << monster.prepareWrite();
	#endif


/*****************************************************************************\
 *                                WRITE FILE                                 *
\*****************************************************************************/
	/*
	findAndReplace(monster.file, PFSB_VERSION,u8"PROGVER",monster.fileName);

	tempStorage = "PFSB - ";
	tempStorage += monster.monName;
	findAndReplace(monster.file,tempStorage.c_str(),u8"PF_MON",monster.fileName);
	#ifdef _DEBUG
	cout << "Monster header written" << endl;
	#endif

	findAndReplace(monster.file,monster.monName.c_str(),u8"MONNAME",monster.fileName);
	#ifdef _DEBUG
	cout << "Monname written" << endl;
	#endif

	findAndReplace(monster.file,monster.cr,u8"CR",monster.fileName);
	#ifdef _DEBUG
	cout << "CR written" << endl;
	#endif
	findAndReplace(monster.file,(monster.xp.c_str()),u8"XP",monster.fileName);
	#ifdef _DEBUG
	cout << "XP written" << endl;
	cout << endl << endl << endl << "Value (c++-string): " << monster.xp << endl << endl << endl;
	cout << endl << endl << endl << "Value (c - string): " << monster.xp.c_str() << endl << endl << endl;
	#endif
	findAndReplace(monster.file,monster.alignment,u8"ALIGN",monster.fileName);
	#ifdef _DEBUG
	cout << "Alignment written" << endl;
	#endif
	findAndReplace(monster.file,monster.creatSizeWords.c_str(),u8"SIZE",monster.fileName);
	#ifdef _DEBUG
	cout << "Size written: " << monster.creatSizeWords << endl;
	#endif

	findAndReplace(monster.file,monster.creatTypeWords.c_str(),u8"CREATTY",monster.fileName);
	findAndReplace(monster.file,monster.creatTypeWords.c_str(),u8"CREATTY",monster.fileName);

	findAndReplace(monster.file,monster.subtypes.c_str(),u8"SUBTYPE",monster.fileName);
	findAndReplace(monster.file,to_string(monster.initiative).c_str(),u8"INIT",monster.fileName);
	findAndReplace(monster.file,monster.senses.c_str(),u8"SENSES",monster.fileName);
	findAndReplace(monster.file,monster.aura.c_str(),u8"AURA",monster.fileName);




	findAndReplace(monster.file,to_string(monster.ac.totalAc).c_str(),u8"REGAC",monster.fileName);
	tempStorage = "touch ";
	findAndReplace(monster.file,(tempStorage + to_string(monster.ac.touchAc)).c_str(),u8"TOUCHAC",monster.fileName);
	tempStorage = "flat-footed ";
	findAndReplace(monster.file,(tempStorage + to_string(monster.ac.flatFootAc)).c_str(),u8"FLATFOO",monster.fileName);
	findAndReplace(monster.file,monster.ac.breakdown.c_str(),u8"ACBREAK",monster.fileName);

	findAndReplace(monster.file,to_string(monster.hp).c_str(),u8"HP",monster.fileName);
	findAndReplace(monster.file,monster.hpBreakdown.c_str(),u8"HPBREAK",monster.fileName);

	findAndReplace(monster.file,to_string(monster.saves.fort).c_str(),u8"FORTSAV",monster.fileName);
	findAndReplace(monster.file,to_string(monster.saves.reflex).c_str(),u8"REFSAVE",monster.fileName);
	findAndReplace(monster.file,to_string(monster.saves.will).c_str(),u8"WILLSAV",monster.fileName);

	findAndReplace(monster.file,monster.defensiveAbilities.c_str(),u8"DEFABLE",monster.fileName);
	findAndReplace(monster.file,monster.immunities.c_str(),u8"IMMUNE",monster.fileName);
	findAndReplace(monster.file,monster.resistances.c_str(),u8"RESIST",monster.fileName);
	findAndReplace(monster.file,monster.weaknesses.c_str(),u8"WEAK",monster.fileName);

	findAndReplace(monster.file,monster.dr.c_str(),u8"DR",monster.fileName);
	findAndReplace(monster.file,monster.sr.c_str(),u8"SR",monster.fileName);




	findAndReplace(monster.file,monster.speed.c_str(),u8"SPEED",monster.fileName);

	findAndReplace(monster.file,monster.meleeAtk.c_str(),u8"MELEE",monster.fileName);
	findAndReplace(monster.file,monster.rangedAtk.c_str(),u8"RANGED",monster.fileName);

	findAndReplace(monster.file,monster.space.c_str(),u8"SPACE",monster.fileName);
	findAndReplace(monster.file,monster.reach.c_str(),u8"REACH",monster.fileName);

	findAndReplace(monster.file,monster.specAtk.c_str(),u8"SPECATK",monster.fileName);




	findAndReplace(monster.file,monster.abilities.displayStr.c_str(),u8"STR",monster.fileName);
	findAndReplace(monster.file,monster.abilities.displayDex.c_str(),u8"DEX",monster.fileName);
	findAndReplace(monster.file,monster.abilities.displayCon.c_str(),u8"CON",monster.fileName);
	findAndReplace(monster.file,monster.abilities.displayInt.c_str(),u8"INT",monster.fileName);
	findAndReplace(monster.file,monster.abilities.displayWis.c_str(),u8"WIS",monster.fileName);
	findAndReplace(monster.file,monster.abilities.displayCha.c_str(),u8"CHA",monster.fileName);

	findAndReplace(monster.file,to_string(monster.bab).c_str(),u8"BAB",monster.fileName);

	findAndReplace(monster.file,to_string(monster.maneuvers.cmb).c_str(),u8"CMB",monster.fileName);
	findAndReplace(monster.file,to_string(monster.maneuvers.cmd).c_str(),u8"CMD",monster.fileName);
	findAndReplace(monster.file,monster.maneuvers.specialCmb.c_str(),u8"SPECCMB",monster.fileName);
	findAndReplace(monster.file,monster.maneuvers.specialCmd.c_str(),u8"SPECCMD",monster.fileName);

	findAndReplace(monster.file,monster.featList.c_str(),u8"FEATLIS",monster.fileName);
	findAndReplace(monster.file,monster.skillList.c_str(),u8"SKILIST",monster.fileName);
	findAndReplace(monster.file,monster.languageList.c_str(),u8"LANGLIS",monster.fileName);

	findAndReplace(monster.file,monster.specialQualities.c_str(),u8"SQLIST",monster.fileName);




	findAndReplace(monster.file,monster.environmentList.c_str(),u8"ENVLIST",monster.fileName);
	findAndReplace(monster.file,monster.groupList.c_str(),u8"GRPLIST",monster.fileName);
	findAndReplace(monster.file,monster.lootList.c_str(),u8"LOOTLST",monster.fileName);




	findAndReplace(monster.file,monster.specialAbilities.c_str(),u8"SPECABL",monster.fileName);
	*/


	monster.file.close();

	#ifdef _DEBUG
	cout << "Monster closed" << endl;
	#endif


	//cout << monName << endl << cr << endl << displayCr << endl << xp << endl << creatSizeWords << endl << creatSize << endl << creatType << endl << subtypes << endl;

}

void printHelp(const po::options_description& desc) {

	cout << desc << endl;
	cout << "Run the program with no arguments to use it. It will output an HTML file in the current directory." << endl;
	cout << "Note: It is strongly advised that no user inputs contain dollar signs."<< endl << endl;
}

void printOgl() {
	cout << "Product Identity: The following items are hereby identified as Product Identity," << endl;
	cout << "as defined in the Open Game License version 1.0a, Section 1(e), and are not Open" << endl;
	cout << "Content: All trademarks, registered trademarks, proper names (characters," << endl;
	cout << "deities, etc.), dialogue, plots, storylines, locations, characters, artwork, and" << endl;
	cout << "trade dress. (Elements that have previously been designated as Open Game Content" << endl;
	cout << "or are in the public domain are not included in this declaration.)" << endl << endl;

	cout << "Open Content: Except for material designated as Product Identity (see above)," << endl;
	cout << "the game mechanics of this game product are Open Game Content, as defined in the" << endl;
	cout << "Open Gaming License version 1.0a Section 1(d). No portion of this work other" << endl;
	cout << "than the material designated as Open Game Content may be reproduced in any form" << endl;
	cout << "without written permission." << endl << endl;
}

void printVersion() {
	cout << "PFSB, a PF RPG monster Stat Block generator" << endl << endl;
	cout << "PFSB version " << PFSB_VERSION << endl;
	cout << "Distributed under the Open Gaming License v1.0a. A copy of the OGL should have" << endl;
	cout << "been included with this program, as indicated in section 10 of OGLv1.0a, as" << endl;
	cout << "should a copy of the CONTRIBUTORS text file." << endl;
	cout << endl << "Copyright (C) 2017 Frozen Mustelid and contributors"<< endl;
}
