#ifndef PFMON_HPP
#define PFMON_HPP

#include "pfMeta.hpp"

#include <fstream>

class PfMon
{
public:

/* Note: I have to declare most of the data members as public, or else the find-
 * replace function won't work. I'd end up being forced to write trivial getters
 * for nearly every data member. So just make sure that only this class itself
 * (and even then only the functions that should change that specific member's
 * values) ever modifies any data members. Or else we're gonna have big trouble
 * when/if I update this program to support non-PF games.
 * 
 * Also note that these functions always return a value. Assigning this value to
 * the appropriate data member is redundant. The value is returned for
 * a) Debugging purposes, b) In case it becomes necessary in the future to
 * use this value for something else, and c) Style reasons. I believe that void
 * functions should be used sparingly, as they can obfuscate the purpose of a
 * function. Add in the obfuscation caused by method calls that don't take
 * arguments like a function does, and things can get confusing quickly.
 * 
 * On the function naming scheme: "fetch" functions focus almost exclusively on
 * user input. "calculate" functions  do not contain any user input elements at
 * all. "determine" functions feature both, but does enough to not warrant a
 * fetch label.
 */
/******************************************************************************\
 *                                 HEADER                                     *
\******************************************************************************/

	std::string monName;
	ChallengeRating CR;
	uint32_t XP;
	Alignment alignment;
	Size creatSize;
	CreatureType creatType;
	std::string creatTypeWords;
	std::string subtypes;
	int initiative;
	std::string senses;
	std::string aura;

	ChallengeRating fetchCR();
	uint32_t determineXP(const ChallengeRating& cr);
	Alignment fetchAlignment();
	Size fetchSize();
	std::string fetchCreatureType();
	std::string fetchSubtypes();
	int determineInitiative();
	std::string fetchSenses();
	std::string fetchAura();


/******************************************************************************\
 *                                 DEFENSE                                    *
\******************************************************************************/

	AC ac;
	HitDice hd;
	int hp;
	std::string hpBreakdown;
	Saves saves;
	std::string defensiveAbilities;
	std::string immunities;
	std::string resistances;
	std::string weaknesses;
	std::string dr;
	std::string sr;

	AC determineAC();
	HitDice fetchHd();
	int calculateHp();
	Saves determineSaves();
	std::string fetchDefensiveAbilities();
	std::string determineImmunities();
	std::string fetchResistances();
	std::string fetchWeaknesses();
	std::string fetchDr();
	std::string fetchSr();


/******************************************************************************\
 *                                OFFENSE                                     *
\******************************************************************************/

	std::string speed;
	std::string meleeAtk;
	std::string rangedAtk;
	std::string space;
	std::string reach;
	std::string specAtk;

	std::string fetchSpeed();
	std::string fetchMeleeAtk();
	std::string fetchRangedAtk();
	std::string calculateSpace();
	std::string determineReach();
	std::string fetchSpecialAttacks();


/******************************************************************************\
 *                               STATISTICS                                   *
\******************************************************************************/

	AbilityScore strength;
	AbilityScore dexterity;
	AbilityScore constitution;
	AbilityScore intelligence;
	AbilityScore wisdom;
	AbilityScore charisma;
	int bab;
	Maneuvers maneuvers;
	std::string featList;
	std::string skillList;
	std::string languageList;
	std::string specialQualities;

	int calculateBab();
	Maneuvers determineCombatManeuvers();
	std::string fetchFeats();
	std::string fetchSkills();
	std::string fetchLanguages();
	std::string fetchSpecialQualities();

/******************************************************************************\
 *                                ECOLOGY                                     *
\******************************************************************************/

	std::string environmentList;
	std::string groupList;
	std::string lootList;

	std::string fetchEnvironment();
	std::string fetchOrganization();
	std::string fetchTreasure();

/******************************************************************************\
 *                           SPECIAL ABILITIES                                *
\******************************************************************************/

	std::string specialAbilities;

	std::string fetchSpecialAbilities();

/******************************************************************************\
 *                                 SYSTEM                                     *
\******************************************************************************/

	const char* fileName;
	fstream file;

	PfMon();
	~PfMon();
	std::string prepareWrite();


private:
	
/******************************************************************************\
 *                                 HEADER                                     *
\******************************************************************************/

	std::string formatXp(long xp);
	CreatureType determineCreatureType();

/******************************************************************************\
 *                                 DEFENSE                                    *
\******************************************************************************/

	std::string calculateAcBreakdown();
	std::string calculateHpBreakdown();
};

#endif // PFMON_HPP
