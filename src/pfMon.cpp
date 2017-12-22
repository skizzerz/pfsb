#include "pfMon.hpp"

#include <cmath>
#include <cstring>
#include <climits>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

using namespace std;

static map<string, Size> sizeMap{
	{ "F", Size::Fine },
	{ "D", Size::Diminutive },
	{ "T", Size::Tiny },
	{ "S", Size::Small },
	{ "M", Size::Medium },
	{ "L", Size::Large },
	{ "H", Size::Huge },
	{ "G", Size::Gargantuan },
	{ "C", Size::Colossal }
};

static map<string, Alignment> alignmentMap{
	{ "LG", Alignment::LawfulGood },
	{ "NG", Alignment::NeutralGood },
	{ "CG", Alignment::ChaoticGood },
	{ "LN", Alignment::LawfulNeutral },
	{ "N", Alignment::Neutral },
	{ "CN", Alignment::ChaoticNeutral },
	{ "LE", Alignment::LawfulEvil },
	{ "NE", Alignment::NeutralEvil },
	{ "CE", Alignment::ChaoticEvil }
};

static map<string, const ChallengeRating> crMap{
	{ "0", ChallengeRating(0) },
	{ "1/8", ChallengeRating(1, 8) },
	{ "1/6", ChallengeRating(1, 6) },
	{ "1/4", ChallengeRating(1, 4) },
	{ "1/3", ChallengeRating(1, 3) },
	{ "1/2", ChallengeRating(1, 2) },
	{ "1", ChallengeRating(1) },
	{ "2", ChallengeRating(2) },
	{ "3", ChallengeRating(3) },
	{ "4", ChallengeRating(4) },
	{ "5", ChallengeRating(5) },
	{ "6", ChallengeRating(6) },
	{ "7", ChallengeRating(7) },
	{ "8", ChallengeRating(8) },
	{ "9", ChallengeRating(9) },
	{ "10", ChallengeRating(10) },
	{ "11", ChallengeRating(11) },
	{ "12", ChallengeRating(12) },
	{ "13", ChallengeRating(13) },
	{ "14", ChallengeRating(14) },
	{ "15", ChallengeRating(15) },
	{ "16", ChallengeRating(16) },
	{ "17", ChallengeRating(17) },
	{ "18", ChallengeRating(18) },
	{ "19", ChallengeRating(19) },
	{ "20", ChallengeRating(20) },
	{ "21", ChallengeRating(21) },
	{ "22", ChallengeRating(22) },
	{ "23", ChallengeRating(23) },
	{ "24", ChallengeRating(24) },
	{ "25", ChallengeRating(25) },
	{ "26", ChallengeRating(26) },
	{ "27", ChallengeRating(27) },
	{ "28", ChallengeRating(28) },
	{ "29", ChallengeRating(29) },
	{ "30", ChallengeRating(30) }
};

static map<string, uint32_t> xpMap{
	{ "0", 0 },
	{ "1/8", 50 },
	{ "1/6", 65 },
	{ "1/4", 100 },
	{ "1/3", 135 },
	{ "1/2", 200 },
	{ "1", 400 },
	{ "2", 600 },
	{ "3", 800 },
	{ "4", 1'200 },
	{ "5", 1'600 },
	{ "6", 2'400 },
	{ "7", 3'200 },
	{ "8", 4'800 },
	{ "9", 6'400 },
	{ "10", 9'600 },
	{ "11", 12'800 },
	{ "12", 19'200 },
	{ "13", 25'600 },
	{ "14", 38'400 },
	{ "15", 51'200 },
	{ "16", 76'800 },
	{ "17", 102'400 },
	{ "18", 153'600 },
	{ "19", 204'800 },
	{ "20", 307'200 },
	{ "21", 409'600 },
	{ "22", 614'400 },
	{ "23", 819'200 },
	{ "24", 1'228'800 },
	{ "25", 1'638'400 },
	{ "26", 2'457'600 },
	{ "27", 3'276'800 },
	{ "28", 4'915'200 },
	{ "29", 6'553'600 },
	{ "30", 9'830'400 }
};

/**
 * Return a comma-separated list of the map keys.
 */
template<typename Container>
static string map_keys(Container cont) {
	vector<string> valid;
	transform(cont.cbegin(), cont.cend(), std::back_inserter(valid), [](auto pair) { return pair.first; });

	return boost::algorithm::join(valid, ", ");
}


/******************************************************************************\
 *                                 HEADER                                     *
\******************************************************************************/

ChallengeRating PfMon::fetchCR() {
	string line;

	while (true) {
		cout << "CR (fraction if less than 1): ";
		getline(cin, line);

		try {
			return crMap.at(line);
		} catch (const std::out_of_range&) {
			cout << "Invalid CR, please specify a number 0-30 or the fractions 1/8, 1/6, 1/4, 1/3, or 1/2." << endl;
		}
	}
}

uint32_t PfMon::determineXP(const ChallengeRating& cr) {
	return xpMap.at(cr.str());
}

Alignment PfMon::fetchAlignment() {
	string line;

	while (true) {
		cout << "Alignment (abbreviated): ";
		getline(cin, line);
		boost::to_upper(line);

		try {
			return alignmentMap.at(line);
		} catch (const std::out_of_range&) {
			cout << "Invalid alignment, please specify one of: " << map_keys(alignmentMap) << endl;
		}
	}
}

Size PfMon::fetchSize() {
	string line;

	while (true) {
		cout << "Size (single letter): ";
		getline(cin, line);
		boost::to_upper(line);

		try {
			return sizeMap.at(line);
		} catch (const std::out_of_range&) {
			vector<string> valid(sizeMap.size());
			transform(sizeMap.cbegin(), sizeMap.cend(), valid.begin(), [](auto pair) { return pair.first; });
			cout << "Invalid creature size, please specify one of: " << map_keys(sizeMap) << endl;
		}
	}
}

string PfMon::fetchCreatureType() {
	
	cout << "Creature Type: ";
	getline(cin, creatTypeWords);

	creatTypeWords = stringToLower(creatTypeWords);
	
	determineCreatureType();
	
	return creatTypeWords;
}

string PfMon::fetchSubtypes() {
	
	string holding;
	
	cout << "Subtypes (Just hit Enter if none): ";
	getline(cin,holding);
	
	if (!(holding.empty())) {
		subtypes = '(';
		subtypes += holding;
		subtypes += ')';
	}
	
	return subtypes;
}

int PfMon::determineInitiative() {

	int initBonus = abilities.dexMod;
	char choice;

	cout << "Improved initiative feat? (yes/no)" << endl;
	cin >> choice;

	clearBuffer();

	if (choice == 'y' || choice == 'Y') {
		initBonus += 4;
		featList += "Improved Initiative, ";
	}

	initiative = initBonus;
	return initBonus;

}

string PfMon::fetchSenses() {

	int maxPerception = 3 + abilities.wisMod + hd.totalHd;

	cout << "Type what should appear next to the Senses line in the general info section. The max Perception possible, with only \
Wis mod + skill ranks + class skill bonus is " << maxPerception << "." << endl;
	getline(cin,senses);

	return senses;
}

string PfMon::fetchAura() {

	cout << "List the monster's aura/auras here: ";
	getline(cin,aura);

	return aura;
}

string PfMon::formatXp(long xp) {

	string xpString = to_string(xp);
	string xpReversed = "";

	for (unsigned int counter = 0; counter <= xpString.length(); counter++) {
	/* Unsigned to avoid comparing signed int to unsigned int, which is the type
	 * returned by string::length(). Unlikely to cause issues, but all code in
	 * a release build must build without errors or warnings.
	 */
		xpReversed += xpString[xpString.length() - counter];

		if ((((counter) % 3) == 0) && (counter != 0)) {
			xpReversed += ',';
		}
	}

	xpString = "";

	for (unsigned int counter = 0; counter <= xpReversed.length(); counter++) {
		xpString += xpReversed[xpReversed.length() - counter];
	}

	xpString[0] = ' ';

	if (xpString[1] == ',') { //This does produce extra spaces, but it doesnt look like browsers render these.
		xpString[1] = ' ';
	}

	this->xp = xpString;
	return xpString;
}

CreatureType PfMon::determineCreatureType() {

	if (creatTypeWords == "aberration" || creatTypeWords == "abberration" || creatTypeWords == "abberation") {
		creatType = CreatureType::Abberation;
		return CreatureType::Abberation;
	}
	if (creatTypeWords == "animal") {
		creatType = CreatureType::Animal;
		return CreatureType::Animal;
	}
	if (creatTypeWords == "construct") {
		creatType = CreatureType::Construct;
		return CreatureType::Construct;
	}
	if (creatTypeWords == "dragon") {
		creatType = CreatureType::Dragon;
		return CreatureType::Dragon;
	}
	if (creatTypeWords == "fey") {
		creatType = CreatureType::Fey;
		return CreatureType::Fey;
	}
	if (creatTypeWords == "humanoid") {
		creatType = CreatureType::Humanoid;
		return CreatureType::Humanoid;
	}
	if (creatTypeWords == "magical beast") {
		creatType = CreatureType::MagicalBeast;
		return CreatureType::MagicalBeast;
	}
	if (creatTypeWords == "monstrous humanoid") {
		creatType = CreatureType::MonstrousHumanoid;
		return CreatureType::MonstrousHumanoid;
	}
	if (creatTypeWords == "ooze") {
		creatType = CreatureType::Ooze;
		return CreatureType::Ooze;
	}
	if (creatTypeWords == "outsider") {
		creatType = CreatureType::Outsider;
		return CreatureType::Outsider;
	}
	if (creatTypeWords == "plant") {
		creatType = CreatureType::Plant;
		return CreatureType::Plant;
	}
	if (creatTypeWords == "vermin") {
		creatType = CreatureType::Vermin;
		return CreatureType::Vermin;
	}
	if (creatTypeWords == "undead") {
		creatType = CreatureType::Undead;
		return CreatureType::Undead;
	}

	cout << "Creature type not known." << endl;
	cout << "Ability to use homebrew creature types will be in a future version of this program. Exiting program." << endl;
	exit(0);

	return static_cast<CreatureType>(NULL);
}

/******************************************************************************\
 *                                 DEFENSE                                    *
\******************************************************************************/

AC PfMon::determineAC() {

	ac.totalAc = 10;

	cout << "Indicate total bonuses here. For example, if your creature has +5 Plate, give 14 as the armor bonus." << endl;

	cout << "Armor bonus (0 if n/a): ";
	cin >> ac.armor;

	clearBuffer();
	cout << "Shield bonus: ";
	cin >> ac.shield;

	clearBuffer();
	cout << "Natural armor: ";
	cin >> ac.natural;

	clearBuffer();
	cout << "Deflection bonus: ";
	cin >> ac.deflection;

	clearBuffer();
	cout << "Dodge bonus: ";
	cin >> ac.dodge;

	clearBuffer();
	cout << "Sacred: ";
	cin >> ac.sacred;

	clearBuffer();
	cout << "Profane: ";
	cin >> ac.profane;

	clearBuffer();
	cout << "Insight: ";
	cin >> ac.insight;

	clearBuffer();
	cout << "Luck: ";
	cin >> ac.luck;

	clearBuffer();
	cout << "Racial: ";
	cin >> ac.racial;

	clearBuffer();//
	cout << "Untyped: ";
	cin >> ac.untyped;

	ac.dex = abilities.dexMod;

	switch (creatSize) {
		case Size::Fine: {
			ac.size = 8;
			break;
		}
		case Size::Diminutive: {
			ac.size = 4;
			break;
		}
		case Size::Tiny: {
			ac.size = 2;
			break;
		}
		case Size::Small: {
			ac.size = 1;
			break;
		}
		case Size::Medium: {
			ac.size = 0;
			break;
		}
		case Size::Large: {
			ac.size = -1;
			break;
		}
		case Size::Huge: {
			ac.size = -2;
			break;
		}
		case Size::Gargantuan: {
			ac.size = -4;
			break;
		}
		case Size::Colossal: {
			ac.size = -8;
			break;
		}
		default: {
			cout << "Invalid creature size, exiting program." << endl;
			exit(_ERR_INVALID_SIZE);
		}

	}

	ac.totalAc += (ac.armor + ac.deflection + ac.dex + ac.dodge + ac.insight + ac.luck + ac.natural + ac.profane + ac.racial + ac.sacred + ac.shield + ac.size + ac.trait + ac.untyped);

	ac.touchAc = ac.totalAc - (ac.armor +  ac.shield + ac.natural);

	ac.flatFootAc = ac.totalAc - (ac.dex + ac.dodge);

	/*#ifdef _DEBUG
	cout << "After calculations" << endl;
	cout << "Armor bonus (0 if n/a): " << to_string(ac.armor) << endl;
	cout << "Shield bonus: " << to_string(ac.shield) << endl;
	cout << "Natural armor: " << to_string(ac.natural) << endl;
	cout << "Deflection bonus: " << to_string(ac.deflection) << endl;
	cout << "Dodge bonus: " << to_string(ac.dodge) << endl;
	cout << "Sacred: " << to_string(ac.sacred) << endl;
	cout << "Profane: " << to_string(ac.profane) << endl;
	cout << "Insight: " << to_string(ac.insight) << endl;
	cout << "Luck: " << to_string(ac.luck) << endl;
	cout << "Racial: " << to_string(ac.racial) << endl;
	cout << "Untyped: " << to_string(ac.untyped) << endl;

	cout << "Dex bonus: " << to_string(ac.dex) << endl;
	cout << "Size bonus: " << to_string(ac.size) << endl;

	cout << "Total AC: " << to_string(ac.totalAc) << endl;
	cout << "Flat-footed AC: " << to_string(ac.flatFootAc) << endl;
	cout << "Touch AC: " << to_string(ac.touchAc) << endl;

	cout << "AC Breakdown: " << ac.breakdown << endl;
	#endif*/

	ac.breakdown = calculateAcBreakdown();

	/*#ifdef _DEBUG
	cout << "After AC breakdown calculated" << endl;
	cout << "Armor bonus (0 if n/a): " << to_string(ac.armor) << endl;
	cout << "Shield bonus: " << to_string(ac.shield) << endl;
	cout << "Natural armor: " << to_string(ac.natural) << endl;
	cout << "Deflection bonus: " << to_string(ac.deflection) << endl;
	cout << "Dodge bonus: " << to_string(ac.dodge) << endl;
	cout << "Sacred: " << to_string(ac.sacred) << endl;
	cout << "Profane: " << to_string(ac.profane) << endl;
	cout << "Insight: " << to_string(ac.insight) << endl;
	cout << "Luck: " << to_string(ac.luck) << endl;
	cout << "Racial: " << to_string(ac.racial) << endl;
	cout << "Untyped: " << to_string(ac.untyped) << endl;

	cout << "Dex bonus: " << to_string(ac.dex) << endl;
	cout << "Size bonus: " << to_string(ac.size) << endl;

	cout << "Total AC: " << to_string(ac.totalAc) << endl;
	cout << "Flat-footed AC: " << to_string(ac.flatFootAc) << endl;
	cout << "Touch AC: " << to_string(ac.touchAc) << endl;

	cout << "AC Breakdown: " << ac.breakdown << endl;
	#endif*/

	clearBuffer();

	return ac;

}

HitDice PfMon::fetchHd() {

	int racialHd;

	cout << "How many racial HD does this creature have? ";
	cin >> racialHd;
	hd.totalHd = racialHd;
	hd.racialHdCount = hd.totalHd;

	cout << "Class levels for monsters will come in a later version." << endl;

	switch (creatType) {
		case CreatureType::Abberation: {
			hd.d8 = racialHd;
			break;
		}
		case CreatureType::Animal: {
			hd.d8 = racialHd;
			break;
		}
		case CreatureType::Construct: {
			hd.d10 = racialHd;
			switch (creatSize) {
				case Size::Fine:
				case Size::Diminutive:
				case Size::Tiny: {
					break;
				}
				case Size::Small: {
					hd.bonus = 10;
					break;
				}
				case Size::Medium: {
					hd.bonus = 20;
					break;
				}
				case Size::Large: {
					hd.bonus = 30;
					break;
				}
				case Size::Huge: {
					hd.bonus = 40;
					break;
				}
				case Size::Gargantuan: {
					hd.bonus = 60;
					break;
				}
				case Size::Colossal: {
					hd.bonus = 80;
					break;
				}
				default: {
					cout << "Invalid creature size found, exiting program" << endl;
					exit(_ERR_INVALID_SIZE);
				}
			}
			break;
		}
		case CreatureType::Dragon: {
			hd.d12 = racialHd;
			break;
		}
		case CreatureType::Fey: {
			hd.d6 = racialHd;
			break;
		}
		case CreatureType::Humanoid: {
			hd.d8 = racialHd;
			break;
		}
		case CreatureType::MagicalBeast: {
			hd.d10 = racialHd;
			break;
		}
		case CreatureType::MonstrousHumanoid: {
			hd.d10 = racialHd;
			break;
		}
		case CreatureType::Ooze: {
			hd.d8 = racialHd;
			break;
		}
		case CreatureType::Outsider: {
			hd.d10 = racialHd;
			break;
		}
		case CreatureType::Plant: {
			hd.d8 = racialHd;
			break;
		}
		case CreatureType::Undead: {
			hd.d8 = racialHd;
			break;
		}
		case CreatureType::Vermin: {
			hd.d8 = racialHd;
			break;
		}
		default: {
			cout << "Homebrew creature types will come later. Exiting program." << endl;
			exit(_ERR_INVALID_CREATURE_TYPE);
		}
	}
	
	return hd;
}

int PfMon::calculateHp() {

	double hdAverage = 0.0;
	hp = 0;

	/* Note that even when monsters have multiple HD sources and two or more
	 * sources of hit dice that average to ?.5 each, each source of HD is
	 * rounded down before being added. For proof, math out the Serpentfolk
	 * Bone Prophet on MC204.
	 */

	hdAverage += (2.5 * hd.d4);
	hdAverage += (3.5 * hd.d6);
	hdAverage += (4.5 * hd.d8);
	hdAverage += (5.5 * hd.d10);
	hdAverage += (6.5 * hd.d12);

	hp = hd.bonus + static_cast<int>(hdAverage) + (abilities.conMod * hd.totalHd);


	/*#ifdef _DEBUG
	cout << endl << "determineHP()" << endl << endl;
	cout << "HP total: " << hp << endl;
	cout << "HD average: " << hdAverage << endl;
	cout << "HD average (casted to int): " << static_cast<int>(hdAverage) << endl;
	cout << "Con mod: " << abilities.conMod << endl;
	cout << "Con bonus HP: " << (abilities.conMod * hd.totalHd) << endl;
	cout << "Special bonus HP: " << hd.bonus << endl;
	cout << "Total HD: " << hd.totalHd << endl;
	cout << "Total d4: " << hd.d4 << endl;
	cout << "Total d6: " << hd.d6 << endl;
	cout << "Total d8: " << hd.d8 << endl;
	cout << "Total d10: " << hd.d10 << endl;
	cout << "Total d12: " << hd.d12 << endl;
	#endif*/
	
	calculateHpBreakdown();

	return hp;
}

string PfMon::fetchDefensiveAbilities() {

	cout << "List the monster's defensive abilities here: ";
	getline(cin,defensiveAbilities);

	return defensiveAbilities;
}

string PfMon::determineImmunities() {

	string typeImmunities;

	switch (creatType) {
		case CreatureType::Construct: {
			typeImmunities = "construct traits, ";
			break;
		}
		case CreatureType::Plant: {
			typeImmunities = "plant traits, ";
			break;
		}
		case CreatureType::Undead: {
			typeImmunities = "undead traits, ";
			break;
		}
		default: {
			//doNothing();
		}
	}

	cout << "List the monster's immunities here: ";
	getline(cin,immunities);

	return immunities;
}

string PfMon::fetchResistances() {

	cout << "List the monster's resistances here: ";
	getline(cin,resistances);

	return resistances;
}

string PfMon::fetchWeaknesses() {

	cout << "List the monster's weaknesses here: ";
	getline(cin,weaknesses);

	return weaknesses;
}

string PfMon::fetchDr() {

	cout << "List the monster's DR here: ";
	getline(cin,dr);

	return dr;
}

string PfMon::fetchSr() {

	cout << "List the monster's SR here: ";
	getline(cin,sr);

	return sr;
}

Saves PfMon::determineSaves() {

	char choice;

	switch (creatType) {
		case CreatureType::Abberation: {
			saves.goodWill = true;
			break;
		}
		case CreatureType::Animal: {
			saves.goodFort = true;
			saves.goodReflex = true;
			break;
		}
		case CreatureType::Construct: {
			break;
		}
		case CreatureType::Dragon: {
			saves.goodFort = true;
			saves.goodReflex = true;
			saves.goodWill = true;
			break;
		}
		case CreatureType::Fey: {
			saves.goodFort = true;
			break;
		}
		case CreatureType::Humanoid: {
			cout << "Humanoid creatures get 1 good save." << endl;
			cout << "\t[1] Fortitude" << endl;
			cout << "\t[2] Reflex" << endl;
			cout << "\t[3] Will" << endl;

			cin >> choice;
			clearBuffer();

			switch(choice) {
				case '1': {
					saves.goodFort = true;
					break;
				}
				case '2': {
					saves.goodReflex = true;
					break;
				}
				case '3': {
					saves.goodWill = true;
					break;
				}
				default: {
					cout << "I have no idea what you just typed. Defaulting to all poor saves." << endl;
				}
			}
			break;
		}
		case CreatureType::MagicalBeast: {
			saves.goodFort = true;
			saves.goodReflex = true;
			break;
		}
		case CreatureType::MonstrousHumanoid: {
			saves.goodReflex = true;
			saves.goodWill = true;
			break;
		}
		case CreatureType::Ooze: {
			break;
		}
		case CreatureType::Outsider: {
			cout << "Outsider creatures get 2 good saves." << endl;
			cout << "\t[1] Fortitude  +  Reflex" << endl;
			cout << "\t[2] Fortitude  +  Will" << endl;
			cout << "\t[3] Reflex     +  Will" << endl;

			cin >> choice;
			clearBuffer();

			switch(choice) {
				case '1': {
					saves.goodFort = true;
					saves.goodReflex = true;
					break;
				}
				case '2': {
					saves.goodFort = true;
					saves.goodWill = true;
					break;
				}
				case '3': {
					saves.goodReflex = true;
					saves.goodWill = true;
					break;
				}
				default: {
					cout << "I have no idea what you just typed. Defaulting to all poor saves." << endl;
				}
			}
			break;
		}
		case CreatureType::Plant: {
			saves.goodFort = true;
			break;
		}
		case CreatureType::Undead: {
			saves.goodWill = true;
			break;
		}
		case CreatureType::Vermin: {
			saves.goodFort = true;
			break;
		}
		default: {
			cout << "Homebrew creature types will come later. Exiting program." << endl;
			exit(_ERR_INVALID_CREATURE_TYPE);
		}

	}

	if (saves.goodFort) {
		saves.fort = static_cast<int>(0.5 * hd.totalHd) + 2;
		//cout << "Base Fort: " << saves.fort << endl;
	} else {
		saves.fort = static_cast<int>((1.0 / 3) * hd.totalHd);
		//cout << "Base Fort: " << saves.fort << endl;
	}

	if (saves.goodReflex) {
		saves.reflex = static_cast<int>(0.5 * hd.totalHd) + 2;
		//cout << "Base Ref: " << saves.reflex << endl;
	} else {
		saves.reflex = static_cast<int>((1.0 / 3) * hd.totalHd);
		//cout << "Base Ref: " << saves.reflex << endl;
	}

	if (saves.goodWill) {
		saves.will = static_cast<int>(0.5 * hd.totalHd) + 2;
		//cout << "Base Will: " << saves.will << endl;
	} else {
		saves.will = static_cast<int>((1.0 / 3) * hd.totalHd);
		//cout << "Base Will: " << saves.will << endl;
	}

	saves.fort   += abilities.conMod;
	saves.reflex += abilities.dexMod;
	saves.will   += abilities.wisMod;

	#ifdef _DEBUG
	cout << "Good fort? " << saves.goodFort << endl;
	cout << "Good reflex? " << saves.goodReflex << endl;
	cout << "Good will? " << saves.goodWill << endl;

	cout << "CON mod: " <<  abilities.conMod << endl;
	cout << "DEX mod: " <<  abilities.dexMod << endl;
	cout << "WIS mod: " <<  abilities.wisMod << endl;
	#endif

	 return saves;
}

string PfMon::calculateAcBreakdown() {

	ac.breakdown = "(";

	if (ac.armor != 0) {
		ac.breakdown += to_string(ac.armor);
		ac.breakdown += " armor, ";
	}
	if (ac.shield != 0) {
		ac.breakdown += to_string(ac.shield);
		ac.breakdown += " shield, ";
	}
	if (ac.natural != 0) {
		ac.breakdown += to_string(ac.natural);
		ac.breakdown += " natural, ";
	}
	if (ac.dex != 0) {
		ac.breakdown += to_string(ac.dex);
		ac.breakdown += " Dex, ";
	}
	if (ac.dodge != 0) {
		ac.breakdown += to_string(ac.dodge);
		ac.breakdown += " dodge, ";
	}
	if (ac.deflection != 0) {
		ac.breakdown += to_string(ac.deflection);
		ac.breakdown += " deflection, ";
	}
	if (ac.insight != 0) {
		ac.breakdown += to_string(ac.insight);
		ac.breakdown += " insight, ";
	}
	if (ac.luck != 0) {
		ac.breakdown += to_string(ac.luck);
		ac.breakdown += " luck, ";
	}
	if (ac.sacred != 0) {
		ac.breakdown += to_string(ac.sacred);
		ac.breakdown += " sacred, ";
	}
	if (ac.profane != 0) {
		ac.breakdown += to_string(ac.profane);
		ac.breakdown += " profane, ";
	}
	if (ac.racial != 0) {
		ac.breakdown += to_string(ac.racial);
		ac.breakdown += " racial, ";
	}
	if (ac.size != 0) {
		ac.breakdown += to_string(ac.size);
		ac.breakdown += " size";
	}

	ac.breakdown += ")";
	return ac.breakdown;
}

string PfMon::calculateHpBreakdown() {

	int hdTypes = 0;

	if (hd.d4) {
		hdTypes += 1;
	}
	if (hd.d6) {
		hdTypes += 1;
	}
	if (hd.d8) {
		hdTypes += 1;
	}
	if (hd.d10) {
		hdTypes += 1;
	}
	if (hd.d12) {
		hdTypes += 1;
	}

	if (hdTypes > 1) {
		hpBreakdown = to_string(hd.totalHd);
		hpBreakdown += "HD; ";
	}


	if (hd.d4) {
		hpBreakdown += to_string(hd.d4) + "d4 + ";
	}
	if (hd.d6) {
		hpBreakdown += to_string(hd.d6) + "d6 + ";
	}
	if (hd.d8) {
		hpBreakdown += to_string(hd.d8) + "d8 + ";
	}
	if (hd.d10) {
		hpBreakdown += to_string(hd.d10) + "d10 + ";
	}
	if (hd.d12) {
		hpBreakdown += to_string(hd.d12) + "d12 + ";
	}

	if (creatType == CreatureType::Undead) {
		hpBreakdown += to_string(abilities.chaMod * hd.totalHd + hd.bonus);
	} else if (creatType == CreatureType::Construct) {
		hpBreakdown += to_string(hd.bonus);
	} else {
		hpBreakdown += to_string(abilities.conMod * hd.totalHd + hd.bonus);
	}

	return hpBreakdown;
}

/******************************************************************************\
 *                                OFFENSE                                     *
\******************************************************************************/

string PfMon::fetchSpeed() {

	cout << "List speed here (including alt speeds, like fly or swim): " << endl;
	getline(cin,speed);

	return speed;

}

string PfMon::fetchMeleeAtk() {

	int strAtkBonus = bab + abilities.strMod;
	int finesseAtkBonus = bab + abilities.dexMod;

	cout << "Type what you want to appear next to the Melee Attack line." << endl;
	cout << "The STR-based attack bonus is " << strAtkBonus << ", and the DEX-based attack bonus (if applicable) is " << finesseAtkBonus << endl;

	getline(cin,meleeAtk);

	return meleeAtk;
}

string PfMon::fetchRangedAtk() {

	int thrownAtkBonus = bab + abilities.strMod;
	int bowAtkBonus = bab + abilities.dexMod;

	cout << "Type what you want to appear next to the Ranged Attack line." << endl;
	cout << "The STR-based attack bonus is " << thrownAtkBonus << ", and the DEX-based attack bonus is " << bowAtkBonus << endl;

	getline(cin,rangedAtk);

	return rangedAtk;
}

string PfMon::calculateSpace() {

	switch (creatSize) {
		case Size::Fine: {
			space = "1/2 ft.";
			break;
		}
		case Size::Diminutive: {
			space = "1 ft.";
			break;
		}
		case Size::Tiny: {
			space = "2-1/2 ft.";
			break;
		}
		case Size::Small: {
			space = "5 ft.";
			break;
				}
		case Size::Medium: {
			space = "5 ft.";
			break;
		}
		case Size::Large: {
			space = "10 ft.";
			break;
		}
		case Size::Huge: {
			space = "15 ft.";
			break;
		}
		case Size::Gargantuan: {
			space = "20 ft.";
			break;
		}
		case Size::Colossal: {
			space = "30 ft.";
			break;
		}
		default: {
			cout << "Invalid creature size found, exiting program" << endl;
			exit(_ERR_INVALID_SIZE);
		}
	}

	return space;
}

string PfMon::determineReach() {

	string isTall;

	switch (creatSize) {
		case Size::Fine: {
			reach = "0 ft.";
			break;
		}
		case Size::Diminutive: {
			reach = "0 ft.";
			break;
		}
		case Size::Tiny: {
			reach = "0 ft.";
			break;
		}
		case Size::Small: {
			reach = "5 ft.";
			break;
				}
		case Size::Medium: {
			reach = "5 ft.";
			break;
		}
		case Size::Large: {
			cout << "Is the creature tall or long? ";
			cin >> isTall;

			if (tolower(isTall[0]) == 't') {
				reach = "10 ft.";
			} else {
				reach = "5 ft.";
			}
			break;
		}
		case Size::Huge: {
			cout << "Is the creature tall or long? ";
			cin >> isTall;

			if (tolower(isTall[0]) == 't') {
				reach = "15 ft.";
			} else {
				reach = "10 ft.";
			}
			break;
		}
		case Size::Gargantuan: {
			cout << "Is the creature tall or long? ";
			cin >> isTall;

			if (tolower(isTall[0]) == 't') {
				reach = "20 ft.";
			} else {
				reach = "15 ft.";
			}
			break;
		}
		case Size::Colossal: {
			cout << "Is the creature tall or long? ";
			cin >> isTall;

			if (tolower(isTall[0]) == 't') {
				reach = "30 ft.";
			} else {
				reach = "20 ft.";
			}
			break;
		}
		default: {
			cout << "Invalid creature size found, exiting program" << endl;
			exit(_ERR_INVALID_SIZE);
		}
	}

	return reach;
}

string PfMon::fetchSpecialAttacks() {

	cout << "List the monster's special attacks here: ";
	getline(cin,specAtk);

	return specAtk;
}

/******************************************************************************\
 *                               STATISTICS                                   *
\******************************************************************************/

Abilities PfMon::fetchAbilities() {

	cout << "Give the scores, not the modifiers." << endl;

	cout << "STR: ";
	cin >> abilities.str;
	abilities.strMod = static_cast<int>(floor((abilities.str - 10) / 2.0));
	abilities.displayStr = to_string(abilities.str);


	clearBuffer();
	cout << "DEX: ";
	cin >> abilities.dex;
	abilities.dexMod = static_cast<int>(floor((abilities.dex - 10) / 2.0));
	abilities.displayDex = to_string(abilities.dex);

	if ((creatType != CreatureType::Undead) && (creatType != CreatureType::Construct)) {
		cout << "CON: ";
		cin >> abilities.con;
		abilities.conMod = static_cast<int>(floor((abilities.con - 10) / 2.0));
		abilities.displayCon = to_string(abilities.con);
		/*cout << "determineBaseStats CON score (assign): " << abilities.con << endl;
		cout << "determineBaseStats CON mod: (assign)" << abilities.conMod << endl;*/
	} else {
		abilities.con = INT_MIN;
		abilities.displayCon = "—"; //Em-dash (U+2014) for extra width
	}


	if ((creatType != CreatureType::Construct) && (creatType != CreatureType::Ooze)) {
		clearBuffer();
		cout << "INT: ";
		cin >> abilities.intelligence;
		abilities.intMod = static_cast<int>(floor((abilities.intelligence - 10) / 2.0));
		abilities.displayInt = to_string(abilities.intelligence);
	} else {
		abilities.intelligence = INT_MIN;
		abilities.intMod = INT_MIN;
		abilities.displayInt = "—"; //Em-dash (U+2014) for extra width
	}

	clearBuffer();
	cout << "WIS: ";
	cin >> abilities.wis;
	abilities.wisMod = static_cast<int>(floor((abilities.wis - 10) / 2.0));
	abilities.displayWis = to_string(abilities.wis);

	clearBuffer();
	cout << "CHA: ";
	cin >> abilities.cha;
	abilities.chaMod = static_cast<int>(floor((abilities.cha - 10) / 2.0));
	abilities.displayCha = to_string(abilities.cha);


	if (creatType == CreatureType::Undead) {
		abilities.conMod = abilities.chaMod;
	}
	if (creatType == CreatureType::Construct) {
		abilities.conMod = 0;
	}

#ifdef _DEBUG
	cout << "Abilities: " << endl;

	cout << "STR   score: " << abilities.str << endl;
	cout << "STR     mod: " << abilities.strMod << endl;
	cout << "Display STR: " << abilities.displayStr << endl << endl;

	cout << "DEX   score: " << abilities.dex << endl;
	cout << "DEX     mod: " << abilities.dexMod << endl;
	cout << "Display DEX: " << abilities.displayDex << endl << endl;

	cout << "CON   score: " << abilities.con << endl;
	cout << "STR     CON: " << abilities.conMod << endl;
	cout << "Display CON: " << abilities.displayCon << endl << endl;

	cout << "INT   score: " << abilities.intelligence << endl;
	cout << "INT     mod: " << abilities.intMod << endl;
	cout << "Display INT: " << abilities.displayInt << endl << endl;

	cout << "WIS   score: " << abilities.wis << endl;
	cout << "WIS     mod: " << abilities.wisMod << endl;
	cout << "Display WIS: " << abilities.displayWis << endl << endl;

	cout << "CHA   score: " << abilities.cha << endl;
	cout << "CHA     mod: " << abilities.chaMod << endl;
	cout << "Display CHA: " << abilities.displayCha << endl << endl;
#endif

	return abilities;
}

int PfMon::calculateBab() {

	switch (creatType) {
		case CreatureType::Abberation: {
			bab = static_cast<int>(3.0 / 4 * hd.totalHd);
			break;
		}
		case CreatureType::Animal: {
			bab = static_cast<int>(3.0 / 4 * hd.totalHd);
			break;
		}
		case CreatureType::Construct: {
			bab = hd.totalHd;
			break;
		}
		case CreatureType::Dragon: {
			bab = hd.totalHd;
			break;
		}
		case CreatureType::Fey: {
			bab = static_cast<int>(1.0 / 4 * hd.totalHd);
			break;
		}
		case CreatureType::Humanoid: {
			bab = static_cast<int>(3.0 / 4 * hd.totalHd);
			break;
		}
		case CreatureType::MagicalBeast: {
			bab = hd.totalHd;
			break;
		}
		case CreatureType::MonstrousHumanoid: {
			bab = hd.totalHd;
			break;
		}
		case CreatureType::Ooze: {
			bab = static_cast<int>(3.0 / 4 * hd.totalHd);
			break;
		}
		case CreatureType::Outsider: {
			bab = hd.totalHd;
			break;
		}
		case CreatureType::Plant: {
			bab = static_cast<int>(3.0 / 4 * hd.totalHd);
			break;
		}
		case CreatureType::Undead: {
			bab = static_cast<int>(3.0 / 4 * hd.totalHd);
			break;
		}
		case CreatureType::Vermin: {
			bab = static_cast<int>(3.0 / 4 * hd.totalHd);
			break;
		}
		default: {
			cout << "Homebrew creature types will come later. Exiting program." << endl;
			exit(_ERR_INVALID_CREATURE_TYPE);
		}

	}

	return bab;
}

Maneuvers PfMon::determineCombatManeuvers() {

	char choice;

	maneuvers.cmb = bab;
	maneuvers.cmd = 10 + bab + abilities.strMod + abilities.dexMod;

	cout << "Agile Maneuvers feat? (yes/no)" << endl;
	cin >> choice;

	clearBuffer();

	if (choice == 'y' || choice == 'Y') {
		maneuvers.agileManuevers = true;
		featList += "Agile Maneuvers, ";
	}

	if ((!(maneuvers.agileManuevers)) || (abilities.strMod > abilities.dexMod)) {
		maneuvers.cmb += abilities.strMod;
	} else {
		maneuvers.cmb += abilities.dexMod;
	}

	cout << "Special CMB? (current normal CMB is " << maneuvers.cmb << ')' << endl;
	getline(cin,maneuvers.specialCmb);

	cout << "Special CMD? (current normal CMD is " << maneuvers.cmd << ')' << endl;
	getline(cin, maneuvers.specialCmd);

	return maneuvers;
}

string PfMon::fetchFeats() {

	int hdFeats = (1 + hd.totalHd) / 2;
	string featsAddendum;

	cout << "List the feats here. Based on HD, you should have " << hdFeats << " feats." << endl;
	getline(cin,featsAddendum);

	featList += featsAddendum;

	return featList;
}

string PfMon::fetchSkills() {

	int maxSkillPoints = INT_MIN;

	switch (creatType) {
		case CreatureType::Abberation: {
			maxSkillPoints = (4 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::Animal: {
			maxSkillPoints = (2 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::Construct: {
			maxSkillPoints = (2 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::Dragon: {
			maxSkillPoints = (6 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::Fey: {
			maxSkillPoints = (6 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::Humanoid: {
			maxSkillPoints = (2 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::MagicalBeast: {
			maxSkillPoints = (2 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::MonstrousHumanoid: {
			maxSkillPoints = (4 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::Ooze: {
			maxSkillPoints = (2 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::Outsider: {
			maxSkillPoints = (6 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::Plant: {
			maxSkillPoints = (2 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::Undead: {
			maxSkillPoints = (4 + abilities.intMod) * hd.totalHd;
			break;
		}
		case CreatureType::Vermin: {
			maxSkillPoints = (2 + abilities.intMod) * hd.totalHd;
			break;
		}
		default: {
			cout << "Homebrew creature types will come later. Exiting program." << endl;
			exit(_ERR_INVALID_CREATURE_TYPE);
		}

	}

	 int strSkills = 3 + abilities.strMod + hd.totalHd;
	 int dexSkills = 3 + abilities.dexMod + hd.totalHd;
	 int conSkills = 3 + abilities.conMod + hd.totalHd;
	 int intSkills = 3 + abilities.intMod + hd.totalHd;
	 int wisSkills = 3 + abilities.wisMod + hd.totalHd;
	 int chaSkills = 3 + abilities.chaMod + hd.totalHd;

	 cout << "Enter the creature's skills. The highest possible will be listed below, assuming that the creature gains that skill as a class skill." << endl;
	 cout << "\tSTR: " << strSkills << endl;
	 cout << "\tDEX: " << dexSkills << endl;
	 cout << "\tCON: " << conSkills << endl;
	 cout << "\tINT: " << intSkills << endl;
	cout << "\tWIS: " << wisSkills << endl;
	cout << "\tCHA: " << chaSkills << endl;

	cout << "Total skill ranks available (not yet enforced by program logic): " << maxSkillPoints << endl;
	getline(cin, skillList);

	return skillList;
}

string PfMon::fetchLanguages() {

	cout << "List the monster's languages here: ";
	getline(cin,languageList);

	return languageList;
}

string PfMon::fetchSpecialQualities() {

	cout << "List the monster's special qualities here: ";
	getline(cin,specialQualities);

	return specialQualities;

}

/******************************************************************************\
 *                                ECOLOGY                                     *
\******************************************************************************/

string PfMon::fetchEnvironment() {

	cout << "List the monster's environments here: ";
	getline(cin,environmentList);

	return environmentList;
}

string PfMon::fetchOrganization() {

	cout << "List the monster's grouping habits here: ";
	getline(cin,groupList);

	return groupList;
}

string PfMon::fetchTreasure() {

	cout << "List treasure here: " << endl;
	getline(cin,lootList);

	return lootList;
}

/******************************************************************************\
 *                           SPECIAL ABILITIES                                *
\******************************************************************************/

string PfMon::fetchSpecialAbilities() {

	string abilityName;
	string abilityText;
	string choice;

	cout << "Does this monster have any special abilities not accounted for by the universal monster rules? ";
	getline(cin,choice);

	while (choice[0] == 'y' || choice[0] == 'Y') {

		specialAbilities += "<p><span class=\"itemheader\">";

		cout << "Ability name (Ex/Su/Sp): ";
		getline(cin,abilityName);
		specialAbilities += abilityName;

		specialAbilities += " </span>";

		cout << "Ability text: ";
		getline(cin,abilityText);
		specialAbilities += abilityText;

		specialAbilities += "</p>\n";

		cout << "Continue? ";
		getline(cin,choice);
	}

	return specialAbilities;
}

/******************************************************************************\
 *                                 SYSTEM                                     *
\******************************************************************************/

PfMon::PfMon() {

	string temp;

	cout << "Monster Name: ";
	getline(cin, monName);

	temp = monName;

	fileName = strcat(strcat(strcat(getenv("PWD"),_DIR_SEPARATOR_STRING),temp.c_str()),".html");
	//Compatibility note: I have no idea if $PWD exists on Windows

	cr [0] = '\0';
	cr [1] = '\0';
	cr [2] = '\0';
	cr [3] = '\0';

	alignment[0] = '\0';
	alignment[1] = '\0';
	alignment[2] = '\0';

	senses = "";

	specialAbilities = "";
}

PfMon::~PfMon() {
}

string PfMon::prepareWrite() {

	ifstream monTemplate;

	monTemplate.open(_PFRPG_TEMPLATE, ios::in); 
	file.open(fileName, ios::in | ios::out | ios::trunc);

	file << monTemplate.rdbuf();

	monTemplate.close();

	return "Ready for write!";
}
