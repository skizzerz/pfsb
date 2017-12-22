#include "pfMeta.hpp"

#include <sstream>
#include <algorithm>
#include <numeric>
#include <map>

AbilityScore::AbilityScore() : score_(255), mod_(0) {}

AbilityScore::AbilityScore(uint8_t score) {
	this->score(score);
}

std::string AbilityScore::str(bool includeModifier) const {
	std::stringstream ss;

	if (score_ == 255) {
		// score is not defined
		return "—";
	}

	ss << score_;

	if (includeModifier) {
		ss << " (" << std::showpos << mod_ << ")";
	}

	return ss.str();
}

uint8_t AbilityScore::score() const { return score_; }
int8_t AbilityScore::mod() const { return mod_; }

void AbilityScore::score(uint8_t score) {
	// we narrow uint8_t into int8_t to determine the modifier, and int8_t caps at 127
	if (score > 127) {
		throw std::domain_error("Ability scores cannot be larger than 127.");
	}

	score_ = score;
	mod_ = (static_cast<int8_t>(score) - 10) / 2;
}

ChallengeRating::ChallengeRating() : num_(0), denom_(1) {}

ChallengeRating::ChallengeRating(uint8_t num, uint8_t denom) : num_(num), denom_(denom) {
	if ((denom > 1 && num != 1) || (num == 1 && denom != 2 && denom != 3 && denom != 4 && denom != 6 && denom != 8)) {
		throw std::domain_error("Fractional CR must be 1/8, 1/6, 1/4, 1/3, or 1/2.");
	}

	if (num > 30) {
		throw std::domain_error("CR must be between 0 and 30.");
	}
}

double ChallengeRating::num() const {
	return num_ / static_cast<double>(denom_);
}

std::string ChallengeRating::str() const {
	std::stringstream ss;

	ss << num_;

	if (denom_ > 1) {
		ss << "/" << denom_;
	}

	return ss.str();
}

Bonus::Bonus(BonusType type, int amount, std::string source) : type_(type), amount_(amount), source_(source) {}
BonusType Bonus::type() const { return type_; }
int Bonus::amount() const { return amount_; }
std::string Bonus::source() const { return source_; }

Number::Number(int base) : base_(base) {}

int Number::num() const {
	return num_without({});
}

// to support Number::num_without
static int addUntyped(int init, const std::pair<std::string, int>& pair) {
	return init + pair.second;
}

static int addTyped(int init, const std::pair<BonusType, int>& pair) {
	return init + pair.second;
}

int Number::num_without(const std::unordered_set<BonusType>& types) const {
	std::vector<Bonus> filtered;
	const std::vector<Bonus>* ref;
	std::map<BonusType, int> bonuses;
	std::map<BonusType, int> penalties;
	std::map<std::string, int> untypedBonuses;
	std::map<std::string, int> untypedPenalties;
	int res = base_;

	if (types.size() > 0) {
		// copy the bonus to filtered if its type is not found in types
		std::copy_if(bonuses_.cbegin(), bonuses_.cend(), std::back_inserter(filtered), [&types](auto el) { return types.find(el.type()) == types.end(); });
		ref = &filtered;
	} else {
		// for performance, if no types are specified, use bonuses_ directly
		ref = &bonuses_;
	}

	for (auto el : *ref) {
		if (el.type() == BonusType::Untyped) {
			if (el.amount() < 0) {
				untypedPenalties[el.source()] = std::min(untypedPenalties[el.source()], el.amount());
			} else {
				untypedBonuses[el.source()] = std::max(untypedBonuses[el.source()], el.amount());
			}
		} else {
			if (el.amount() < 0) {
				penalties[el.type()] = std::min(penalties[el.type()], el.amount());
			} else {
				bonuses[el.type()] = std::max(bonuses[el.type()], el.amount());
			}
		}
	}

	res = std::accumulate(untypedPenalties.cbegin(), untypedPenalties.cend(), res, addUntyped);
	res = std::accumulate(untypedBonuses.cbegin(), untypedBonuses.cend(), res, addUntyped);
	res = std::accumulate(penalties.cbegin(), penalties.cend(), res, addTyped);
	res = std::accumulate(bonuses.cbegin(), bonuses.cend(), res, addTyped);

	return res;
}
