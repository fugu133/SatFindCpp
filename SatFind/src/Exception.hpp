/**
 * @file Exception.hpp
 * @author fugu133
 * @brief
 * @version 0.1
 * @date 2023-11-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <stdexcept>
#include <string>

#include "Essential.hpp"

SATFIND_NAMESPACE_BEGIN

class BaseException : public std::runtime_error {
  public:
	BaseException() = delete;
	BaseException(const std::string& what_message, int error_code)
	  : std::runtime_error("[OrbitException]: " + what_message), m_error_code(error_code) {}

	int getReturnCode() const { return m_error_code; }

  private:
	int m_error_code;
};

class TleException : public BaseException {
  public:
	TleException() = delete;
	TleException(const std::string& what_message, int error_code) : BaseException(what_message, error_code) {}

	enum {
		InvalidTleName,
		InvalidTle1,
		InvalidTle2,
		InvalidTleLine,
		UnmatchedCatalogNumber,
		InvalidIntegerString,
		InvalidDoubleString,
		InvalidExponentString
	};
};

class DateTimeException : public BaseException {
  public:
	DateTimeException() = delete;
	DateTimeException(const std::string& what_message, int error_code) : BaseException(what_message, error_code) {}

	enum {
		InvalidYear,
		InvalidMonth,
		InvalidDay,
		InvalidHour,
		InvalidMinute,
		InvalidSecond,
		InvalidMicrosecond,
		InvalidDate,
		InvalidTime,
		InvalidDateTime,
		InvalidIso8601Format
	};
};

class OrbitException : public BaseException {
  public:
	OrbitException() = delete;
	OrbitException(const std::string& what_message, int error_code) : BaseException(what_message, error_code) {}

	enum {
		EccentricityOutOfRange,
		InclinationOutOfRange,
		LongPeriodPredictionError,
		ShortPeriodPredictionError,
		ParameterOutOfRange,
		ObjectDecayed,
	};
};

SATFIND_NAMESPACE_END