/**
 * @file Tle.hpp
 * @author fugu133
 * @brief Tle class
 * @version 0.1
 * @date 2023-11-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "DateTime.hpp"
#include "Essential.hpp"

SATFIND_NAMESPACE_BEGIN

struct TleLineField {
	std::string name;
	std::string tle1;
	std::string tle2;

	TleLineField() : name(""), tle1(""), tle2("") {}

	TleLineField(const std::string& name, const std::string& tle1, const std::string& tle2) : name(name), tle1(tle1), tle2(tle2) {}

	TleLineField(const std::string& tle1, const std::string& tle2) : name(""), tle1(tle1), tle2(tle2) {}

	TleLineField(std::istream& tle) : name(""), tle1(""), tle2("") { read(tle); }

	static constexpr unsigned name_line_length = 24;
	static constexpr unsigned tle_line1_length = 69;
	static constexpr unsigned tle_line2_length = 69;

	std::string toString() const {
		if (name.empty()) {
			return tle1 + "\n" + tle2;
		} else {
			return name + "\n" + tle1 + "\n" + tle2;
		}
	}

	void read(std::istream& tle) {
		bool has_name_line_tle = false;
		std::vector<std::string> tle_lines;

		while (std::getline(tle, tle_lines.emplace_back())) {
			switch (tle_lines.size()) {
				case 1:
					if (tle_lines[0].length() <= TleLineField::name_line_length) {
						has_name_line_tle = true;
					} else if (tle_lines[0].length() == TleLineField::tle_line1_length) {
						has_name_line_tle = false;
					} else {
						throw TleException("Invalid TLE file.", TleException::InvalidTleLine);
					}
					break;
				case 2:
					if (has_name_line_tle) {
						if (tle_lines[1].length() != TleLineField::tle_line1_length) {
							throw TleException("Invalid TLE file.", TleException::InvalidTleLine);
						}
					} else {
						if (tle_lines[1].length() != TleLineField::tle_line2_length) {
							throw TleException("Invalid TLE file.", TleException::InvalidTleLine);
						}
					}
					break;
				case 3:
					if (has_name_line_tle) {
						if (tle_lines[2].length() != TleLineField::tle_line2_length) {
							throw TleException("Invalid TLE file.", TleException::InvalidTleLine);
						}
					}
					break;

				default: throw TleException("Invalid TLE file.", TleException::InvalidTleLine);
			}
		}

		if (has_name_line_tle) {
			name = tle_lines[0];
			tle1 = tle_lines[1];
			tle2 = tle_lines[2];
		} else {
			name = "";
			tle1 = tle_lines[0];
			tle2 = tle_lines[1];
		}
	}

	friend std::istream& operator>>(std::istream& is, TleLineField& tle) {
		tle.read(is);
		return is;
	}

	friend std::ostream& operator<<(std::ostream& os, const TleLineField& tle) {
		os << tle.toString();
		return os;
	}
};

class Tle {
  public:
	Tle() : m_tle_line_field({}) {}

	Tle(const std::string& name, const std::string& tle1, const std::string& tle2) : m_tle_line_field({name, tle1, tle2}) {
		initialize(m_tle_line_field);
	}

	Tle(const std::string& tle1, const std::string& tle2) : m_tle_line_field({"", tle1, tle2}) { initialize(m_tle_line_field); }

	Tle(const TleLineField& tle) : m_tle_line_field(tle) { initialize(m_tle_line_field); }

	Tle(std::istream& tle) : m_tle_line_field(tle) { initialize(m_tle_line_field); }

	auto tleName() const -> std::string { return m_tle_line_field.name; }

	auto tleLine1() const -> std::string { return m_tle_line_field.tle1; }

	auto tleLine2() const -> std::string { return m_tle_line_field.tle2; }

	auto name() const -> std::string { return m_name; }

	auto catalogNumber() const -> int { return m_catalog_number; }

	auto classification() const -> char { return m_classification; }

	auto internationalDesignator() const -> std::string { return m_international_designator; }

	auto& epoch() const { return m_epoch; }

	auto meanMotionD2() const -> double { return m_mean_motion_d2; }

	auto meanMotionDd6() const -> double { return m_mean_motion_dd6; }

	auto bStar() const -> double { return m_bstar; }

	auto ephemerisType() const -> int { return m_ephemeris_type; }

	auto elementNumber() const -> int { return m_element_number; }

	auto inclination() const -> double { return m_inclination; }

	auto rightAscendingNode() const -> double { return m_right_ascension; }

	auto eccentricity() const -> double { return m_eccentricity; }

	auto argumentPerigee() const -> double { return m_argument_perigee; }

	auto meanAnomaly() const -> double { return m_mean_anomaly; }

	auto meanMotion() const -> double { return m_mean_motion; }

	auto revolutionNumber() const -> int { return m_revolution_number; }

	auto toString() const -> std::string {
		std::string ret = "";
		ret += "TLE Name: " + m_name + "\n";
		ret += "TLE Line 1: " + m_tle_line_field.tle1 + "\n";
		ret += "TLE Line 2: " + m_tle_line_field.tle2 + "\n";
		ret += "Catalog Number: " + std::to_string(m_catalog_number) + "\n";
		ret += "Classification: " + std::string(1, m_classification) + "\n";
		ret += "International Designator: " + m_international_designator + "\n";
		ret += "Epoch (UTC): " + m_epoch.toString() + "\n";
		ret += "Mean Motion D2: " + std::to_string(m_mean_motion_d2) + "\n";
		ret += "Mean Motion DD6: " + std::to_string(m_mean_motion_dd6) + "\n";
		ret += "B* Drag Term: " + std::to_string(m_bstar) + "\n";
		ret += "Ephemeris Type: " + std::to_string(m_ephemeris_type) + "\n";
		ret += "Element Set No: " + std::to_string(m_element_number) + "\n";
		ret += "Inclination: " + std::to_string(m_inclination) + "\n";
		ret += "RAAN: " + std::to_string(m_right_ascension) + "\n";
		ret += "Eccentricity: " + std::to_string(m_eccentricity) + "\n";
		ret += "Argument of Perigee: " + std::to_string(m_argument_perigee) + "\n";
		ret += "Mean Anomaly: " + std::to_string(m_mean_anomaly) + "\n";
		ret += "Mean Motion: " + std::to_string(m_mean_motion) + "\n";
		ret += "Revolution Number: " + std::to_string(m_revolution_number) + "\n";
		return ret;
	}

	auto toTleString() const -> std::string {
		std::string ret = "";
		{
			std::string tle_name;
			tle_name = m_name;
			for (unsigned i = 0; i < TleLineField::name_line_length - m_name.length(); i++) {
				tle_name += " ";
			}
			ret += tle_name + "\n";
		}
		ret += m_tle_line_field.tle1 + "\n";
		ret += m_tle_line_field.tle2 + "\n";
		return ret;
	}

	friend std::istream& operator>>(std::istream& is, Tle& tle) {
		TleLineField tle_line_field;
		is >> tle_line_field;
		tle.initialize(tle_line_field);
		return is;
	}

	friend std::ostream& operator<<(std::ostream& os, const Tle& tle) {
		os << tle.toString();
		return os;
	}

  private:
	/* TLE Line */
	TleLineField m_tle_line_field; // TLE 行フィールド

	/* TLE Data */
	std::string m_name;						// オブジェクト名
	int m_catalog_number;					// 衛星カタログ番号
	char m_classification;					// 機密区分
	std::string m_international_designator; // 国際設計識別符号
	DateTime m_epoch;						// 軌道要素UTC元期
	double m_mean_motion_d2;				// 平均運動一次微分係数 (1/2) [rev/day^2]
	double m_mean_motion_dd6;				// 平均運動微二次分係数 (1/6) [rev/day^3]
	double m_bstar;							// B*係数 (SGP4弾道係数)
	int m_ephemeris_type;					// 軌道モデル (not used)
	int m_element_number;					// 要素番号 (not used)
	double m_inclination;					// 軌道傾斜角 [deg]
	double m_right_ascension;				// 昇交点赤経 [deg]
	double m_eccentricity;					// 離心率
	double m_argument_perigee;				// 近地点引数 [deg]
	double m_mean_anomaly;					// 平均近点角 [deg]
	double m_mean_motion;					// 平均運動 [rev/day]
	int m_revolution_number;				// 軌道回数 (not used)

	/* 要素位置 TLE1 */
	static constexpr unsigned tle1_pos_catalog_number = 2;
	static constexpr unsigned tle1_len_catalog_number = 5;
	static constexpr unsigned tle1_pos_classification = 7;
	static constexpr unsigned tle1_len_classification = 1;
	static constexpr unsigned tle1_pos_international_designator = 9;
	static constexpr unsigned tle1_len_international_designator = 8;
	static constexpr unsigned tle1_pos_epoch = 18;
	static constexpr unsigned tle1_len_epoch = 14;
	static constexpr unsigned tle1_pos_epoch_year = 18;
	static constexpr unsigned tle1_len_epoch_year = 2;
	static constexpr unsigned tle1_pos_epoch_day = 20;
	static constexpr unsigned tle1_len_epoch_day = 12;
	static constexpr unsigned tle1_pos_mean_motion_d2 = 33;
	static constexpr unsigned tle1_len_mean_motion_d2 = 10;
	static constexpr unsigned tle1_pos_mean_motion_dd6 = 44;
	static constexpr unsigned tle1_len_mean_motion_dd6 = 8;
	static constexpr unsigned tle1_pos_bstar = 53;
	static constexpr unsigned tle1_len_bstar = 8;
	static constexpr unsigned tle1_pos_ephemeris_type = 62;
	static constexpr unsigned tle1_len_ephemeris_type = 1;
	static constexpr unsigned tle1_pos_element_number = 64;
	static constexpr unsigned tle1_len_element_number = 4;

	/* 要素位置 TLE2 */
	static constexpr unsigned tle2_pos_catalog_number = 2;
	static constexpr unsigned tle2_len_catalog_number = 5;
	static constexpr unsigned tle2_pos_inclination = 8;
	static constexpr unsigned tle2_len_inclination = 8;
	static constexpr unsigned tle2_pos_right_ascension = 17;
	static constexpr unsigned tle2_len_right_ascension = 8;
	static constexpr unsigned tle2_pos_eccentricity = 26;
	static constexpr unsigned tle2_len_eccentricity = 7;
	static constexpr unsigned tle2_pos_argument_perigee = 34;
	static constexpr unsigned tle2_len_argument_perigee = 8;
	static constexpr unsigned tle2_pos_mean_anomaly = 43;
	static constexpr unsigned tle2_len_mean_anomaly = 8;
	static constexpr unsigned tle2_pos_mean_motion = 52;
	static constexpr unsigned tle2_len_mean_motion = 11;
	static constexpr unsigned tle2_pos_revolution_number = 63;
	static constexpr unsigned tle2_len_revolution_number = 5;

	auto isLineValid(const std::string& line, const unsigned number) -> bool {
		switch (number) {

			case 0:
				if (line.length() >= TleLineField::name_line_length) {
					return false;
				} else {
					return true;
				}

			case 1:
				if (line.length() != TleLineField::tle_line1_length) {
					return false;
				} else {
					if (line[0] != '1') {
						return false;
					} else {
						return true;
					}
				}

			case 2:
				if (line.length() != TleLineField::tle_line2_length) {
					return false;
				} else {
					if (line[0] != '2') {
						return false;
					} else {
						return true;
					}
				}

			default: return false;
		}
	}

	auto isSameCatalogNumber(const TleLineField& tle, int& catalog_number) -> bool {
		const auto tle1_cat_num = tle.tle1.substr(tle1_pos_catalog_number, tle1_len_catalog_number);
		const auto tle2_cat_num = tle.tle2.substr(tle2_pos_catalog_number, tle2_len_catalog_number);
		if (tle1_cat_num == tle2_cat_num) {
			catalog_number = toInteger(m_tle_line_field.tle1.substr(tle1_pos_catalog_number, tle1_len_catalog_number));
			return true;
		} else {
			catalog_number = 0;
			return false;
		}
	}

	auto initialize(const TleLineField& tle) -> void {
		/* TLE line validation */
		{
			if (isLineValid(tle.name, 0)) {
				m_tle_line_field.name = tle.name;
			}

			if (!isLineValid(tle.tle1, 1)) {
				throw TleException("Invalid TLE line 1", TleException::InvalidTle1);
			}

			if (!isLineValid(tle.tle2, 2)) {
				throw TleException("Invalid TLE line 2", TleException::InvalidTle2);
			}
		}

		/* カタログ番号 */
		{
			if (!isSameCatalogNumber(tle, m_catalog_number)) {
				throw TleException("Unmatched catalog number", TleException::UnmatchedCatalogNumber);
			}
		}

		/* 機密区分 */
		{ m_classification = tle.tle1[tle1_pos_classification]; }

		/* 国際設計識別符号 */
		{ m_international_designator = tle.tle1.substr(tle1_pos_international_designator, tle1_len_international_designator); }

		/* オブジェクト名 */
		{
			if (!tle.name.empty()) {
				m_name = tle.name;
			} else {
				m_name = tle.tle1.substr(tle1_pos_catalog_number, tle1_len_catalog_number); // オブジェクト名がない場合はカタログ番号を代用
			}
		}

		/* 軌道要素UTC元期 */
		{ m_epoch = toDateTime(tle.tle1.substr(tle1_pos_epoch, tle1_len_epoch)); }

		/* 平均運動一次微分係数 (1/2) [rev/day^2] */
		{ m_mean_motion_d2 = toDouble(tle.tle1.substr(tle1_pos_mean_motion_d2, tle1_len_mean_motion_d2)); }

		/* 平均運動微二次分係数 (1/6) [rev/day^3] */
		{ m_mean_motion_dd6 = toDouble(tle.tle1.substr(tle1_pos_mean_motion_dd6, tle1_len_mean_motion_dd6)); }

		/* B*係数 (SGP4弾道係数) */
		{ m_bstar = toDouble(tle.tle1.substr(tle1_pos_bstar, tle1_len_bstar)); }

		/* 軌道モデル (not used) */
		{ m_ephemeris_type = toInteger(tle.tle1.substr(tle1_pos_ephemeris_type, tle1_len_ephemeris_type)); }

		/* 要素番号 (not used) */
		{ m_element_number = toInteger(tle.tle1.substr(tle1_pos_element_number, tle1_len_element_number)); }

		/* 軌道傾斜角 [deg] */
		{ m_inclination = toDouble(tle.tle2.substr(tle2_pos_inclination, tle2_len_inclination)); }

		/* 昇交点赤経 [deg] */
		{ m_right_ascension = toDouble(tle.tle2.substr(tle2_pos_right_ascension, tle2_len_right_ascension)); }

		/* 離心率 */
		{ m_eccentricity = toDouble("0." + tle.tle2.substr(tle2_pos_eccentricity, tle2_len_eccentricity)); }

		/* 近地点引数 [deg] */
		{ m_argument_perigee = toDouble(tle.tle2.substr(tle2_pos_argument_perigee, tle2_len_argument_perigee)); }

		/* 平均近点角 [deg] */
		{ m_mean_anomaly = toDouble(tle.tle2.substr(tle2_pos_mean_anomaly, tle2_len_mean_anomaly)); }

		/* 平均運動 [rev/day] */
		{ m_mean_motion = toDouble(tle.tle2.substr(tle2_pos_mean_motion, tle2_len_mean_motion)); }

		/* 軌道回数 (not used) */
		{ m_revolution_number = toInteger(tle.tle2.substr(tle2_pos_revolution_number, tle2_len_revolution_number)); }
	}

	auto toInteger(const std::string& str) -> int {
		int result = 0;
		bool in_progress = false;
		for (const auto& c : str) {
			if (std::isdigit(c)) {
				result *= 10;
				result += c - '0';
				in_progress = true;
			} else if (!in_progress && c == ' ') {

			} else {
				throw TleException("Invalid integer string", TleException::InvalidIntegerString);
			}
		}
		if (in_progress) {
			return result;
		} else {
			return 0;
		}
	}

	auto toDouble(const std::string& str) -> double {
		int sign_part = 1;
		int integer_part = 0;
		int decimal_part = 0;
		int exponent_part = 0;
		int divider = 1;
		int pointer = 0;

		enum { SignPart, IntegerPart, DecimalPart, FractionPart, ExponentialPart } state = SignPart;

		while (pointer < (int)str.length()) {
			const auto c = str[pointer];

			switch (state) {
				case SignPart:
					if (c == '-') {
						sign_part = -1;
						pointer++;
					} else if (c == '+' || c == ' ') {
						sign_part = 1;
						pointer++;
					}
					state = FractionPart;
					break;

				case IntegerPart:
					if (std::isdigit(c)) {
						integer_part *= 10;
						integer_part += c - '0';
						divider *= 10;
						pointer++;
					} else if (c == '.') {
						state = DecimalPart;
						divider = 1;
						pointer++;
					} else if (c == '-' || c == '+') {
						state = ExponentialPart;
						decimal_part = integer_part;
						exponent_part = (c == '-') ? -1 : 1;
						integer_part = 0;
						pointer++;
					} else {
						throw TleException("Invalid integer part string", TleException::InvalidDoubleString);
					}
					break;

				case DecimalPart:
					if (std::isdigit(c)) {
						decimal_part *= 10;
						decimal_part += c - '0';
						divider *= 10;
						pointer++;
					} else {
						throw TleException("Invalid decimal part string", TleException::InvalidDoubleString);
					}
					break;

				case FractionPart:
					if (c == '.') {
						state = DecimalPart;
						pointer++;
					} else {
						state = IntegerPart;
					}
					break;

				case ExponentialPart:
					if (std::isdigit(c)) {
						exponent_part *= (c - '0');
						pointer++;
					} else {
						throw TleException("Invalid exponential part string", TleException::InvalidDoubleString);
					}
					break;

				default: throw TleException("Invalid state", TleException::InvalidDoubleString);
			}
		}

		if (state == ExponentialPart) {
			return sign_part * (double)decimal_part / divider * std::pow(10.0, (double)exponent_part);
		} else {
			return sign_part * ((double)integer_part + (double)decimal_part / (double)divider);
		}
	}

	DateTime toDateTime(const std::string& str) {
		std::int32_t year;
		std::int32_t year_digit2 = std::stoi(str.substr(0, tle1_len_epoch_year));
		double days = toDouble(str.substr(2, tle1_len_epoch_day));

		// 2桁の年数を4桁に変換
		if (year_digit2 < 57) {
			year = 2000 + year_digit2;
		} else {
			year = 1900 + year_digit2;
		}

		return DateTime(year, days);
	}
};

SATFIND_NAMESPACE_END
