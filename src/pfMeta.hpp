#ifndef PFMETA_HPP
#define PFMETA_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_set>

enum class CreatureType {
	None = 0,
	Abberation,
	Animal,
	Construct,
	Dragon,
	Fey,
	Humanoid,
	MagicalBeast,
	MonstrousHumanoid,
	Ooze,
	Outsider,
	Plant,
	Undead,
	Vermin
};

enum class Size {
	None = 0,
	Fine,
	Diminutive,
	Tiny,
	Small,
	Medium,
	Large,
	Huge,
	Gargantuan,
	Colossal
};

enum class Alignment {
	None = 0,
	LawfulGood,
	LawfulNeutral,
	LawfulEvil,
	NeutralGood,
	Neutral,
	NeutralEvil,
	ChaoticGood,
	ChaoticNeutral,
	ChaoticEvil
};

enum class BonusType {
	Untyped = 0,
	Alchemical,
	Armor,
	Circumstance,
	Competence,
	Deflection,
	Dodge,
	Enhancement,
	Inherent,
	Insight,
	Luck,
	Morale,
	NaturalArmor,
	Profane,
	Racial,
	Resistance,
	Sacred,
	Shield,
	Size,
	Trait
};

/**
 * Represents an ability score.
 */
class AbilityScore {
private:
	uint8_t score_;
	int8_t mod_;

public:
	/**
	 * Constructs a new AbilityScore initialized to nothing (indicating the creature lacks this score).
	 */
	AbilityScore();

	/**
	 * Constructs a new AbilityScore initialized to the passed-in score.
	 * @param score Score to use
	 */
	AbilityScore(uint8_t score);

	/**
	 * Converts the score into its string representation (the score optionally followed by its modifier in parenthesis).
	 */
	std::string str(bool includeModifier = true) const;

	/**
	 * Returns the score as an unsigned int.
	 */
	uint8_t score() const;

	/**
	 * Sets the AbilityScore to the new score.
	 * @param score Score to set
	 */
	void score(uint8_t score);

	/**
	 * Returns the modifier as a signed int.
	 */
	int8_t mod() const;
};

class ChallengeRating {
private:
	uint8_t num_;
	uint8_t denom_;

public:
	/**
	 * Constructs a new ChallengeRating initialized to CR 0.
	 */
	ChallengeRating();

	/**
	 * Constructs a new ChallengeRating initialized to the specified numerator and denominator.
	 * Only integers 0-30 as well as the fractions 1/8, 1/6, 1/4, 1/3, and 1/2 are allowed.
	 */
	ChallengeRating(uint8_t num, uint8_t denom = 1);

	/**
	 * Returns the challenge rating as a number (double).
	 */
	double num() const;

	/**
	 * Returns the challenge rating as a (fractional) string.
	 */
	std::string str() const;
};

class Bonus {
private:
	BonusType type_;
	int amount_;
	std::string source_; // Untyped bonuses from the same source do not stack

public:
	Bonus(BonusType type, int amount, std::string source);

	BonusType type() const;
	int amount() const;
	std::string source() const;
};

class Number {
private:
	int base_;
	std::vector<Bonus> bonuses_;

public:
	Number(int base);

	Number& add(const Bonus& bonus);

	int num() const;
	int num_without(const std::unordered_set<BonusType>& types) const;
};

struct AC {

	int armor = 0;
	int shield = 0;
	int dex = 0;
	int natural = 0;
	int dodge = 0;
	int deflection = 0;
	int sacred = 0;
	int profane = 0;
	int insight = 0;
	int luck = 0;
	int racial = 0;
	int size = 0;
	int trait = 0;
	int untyped = 0;

	int totalAc = 10;
	int flatFootAc;
	int touchAc;

	std::string breakdown;

};

struct HitDice {
	int d4 = 0;
	int d6 = 0;
	int d8 = 0;
	int d10 = 0;
	int d12 = 0;

	int totalHd;
	int racialHdCount;
	int bonus = 0; //Intended for flat bonuses, like a construct's size bonus. Bonus HP from CON or CHA (if undead) are calculated as needed.

};

struct Maneuvers {
	bool agileManuevers = false;

	int cmb;
	int cmd;

	std::string specialCmb;
	std::string specialCmd;
};

struct Saves {
	bool goodFort = false;
	bool goodReflex = false;
	bool goodWill = false;

	int fort;
	int reflex;
	int will;

};



#endif