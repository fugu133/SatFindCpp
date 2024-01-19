/**
 * @file Macro.hpp
 * @author fugu133
 * @brief マクロ魔術
 * @version 0.1
 * @date 2023-04-30
 * @copyright Copyright (c) 2023
 */

#pragma once

// clang-format off
#define SATFIND_VERSION_MAJOR 1
#define SATFIND_VERSION_MINOR 0
#define SATFIND_VERSION_PATCH 0

#define SATFIND_TO_STRING_EX(x) #x
#define SATFIND_TO_STRING(x) SATFIND_TO_STRING_EX(x)

#define SATFIND_VERSION_CONCAT_EX(major, minor, patch) v ## major.minor.patch
#define SATFIND_VERSION_CONCAT(major, minor, patch) SATFIND_VERSION_CONCAT_EX(major, minor, patch)

#define SATFIND_VERSION_EX \
	SATFIND_VERSION_CONCAT(SATFIND_VERSION_MAJOR, \
				      SATFIND_VERSION_MINOR, \
			              SATFIND_VERSION_PATCH)
#define SATFIND_VERSION SATFIND_VERSION_EX
#define SATFIND_VERSION_STR_EX SATFIND_TO_STRING(SATFIND_VERSION)
#define SATFIND_VERSION_STR SATFIND_VERSION_STR_EX

#define SATFIND_NAMESPACE_VERSION_TAG_CONCAT_EX(major, minor, patch) v ## major ## _ ## minor ## _ ## patch
#define SATFIND_NAMESPACE_VERSION_TAG_CONCAT(major, minor, patch) SATFIND_NAMESPACE_VERSION_TAG_CONCAT_EX(major, minor, patch)
#define SATFIND_NAMESPACE_VERSION_TAG_EX \
	SATFIND_NAMESPACE_VERSION_TAG_CONCAT(SATFIND_VERSION_MAJOR, \
						    SATFIND_VERSION_MINOR, \
					            SATFIND_VERSION_PATCH)
#define SATFIND_NAMESPACE_BASE_TAG satfind
#define SATFIND_NAMESPACE_VERSION_TAG SATFIND_NAMESPACE_VERSION_TAG_EX
#define SATFIND_NAMESPACE_BEGIN \
	namespace SATFIND_NAMESPACE_BASE_TAG {                   \
		inline namespace SATFIND_NAMESPACE_VERSION_TAG {
#define SATFIND_NAMESPACE_END \
	}                                               \
}

#define SATFIND_REQUEST_VERSION_CHECK(major, minor, patch) \
	(SATFIND_VERSION_MAJOR >= major && SATFIND_VERSION_MINOR >= minor && SATFIND_VERSION_PATCH >= patch)

#define SATFIND_REQUEST_VERSION_ASSERTION_MSG_STR(major, minor, patch) "SATFIND-Lib must be has version higher than " SATFIND_TO_STRING(SATFIND_VERSION_CONCAT(major, minor, patch))

#ifndef SATFIND_NO_ASSERTION
#define SATFIND_ASSRET_CONVERTER_REQUEST_VERSION(major, minor, patch) \
	static_assert(SATFIND_REQUEST_VERSION_CHECK(major, minor, patch), SATFIND_REQUEST_VERSION_ASSERTION_MSG_STR(major, minor, patch))
#else
	TELEMETRY_ASSRET_CONVERTER_REQUEST_VERSION(major, minor, patch)
#endif

#define SATFIND_CODE_GEN_CONCAT_EX(tag, type) tag ## _ ## type
#define SATFIND_CODE_GEN_CONCAT(tag, type) SATFIND_CODE_GEN_CONCAT_EX(tag, type)
#define SATFIND_CODE_GEN_TAG koyoh_acs_SATFIND_code_gen
#define SATFIND_CODE_GEN_RESULT_FUNCTION_NAME(x) SATFIND_CODE_GEN_CONCAT(SATFIND_CODE_GEN_TAG, x)
#define SATFIND_CODE_GEN_ARG_STR_T SATFIND_CODE_GEN_CONCAT(SATFIND_NAMESPACE_BASE_TAG, str_t)
#define SATFIND_CODE_GEN_ARG_OBJ_T SATFIND_CODE_GEN_CONCAT(SATFIND_NAMESPACE_BASE_TAG, obj_t)
#define SATFIND_CODE_GEN_ARG_PTR_T SATFIND_CODE_GEN_CONCAT(SATFIND_NAMESPACE_BASE_TAG, ptr_t)
#define SATFIND_CODE_GEN_ARG_OFS_T SATFIND_CODE_GEN_CONCAT(SATFIND_NAMESPACE_BASE_TAG, ofs_t)
#define SATFIND_CODE_GEN_ARG_IPT_T SATFIND_CODE_GEN_CONCAT(SATFIND_NAMESPACE_BASE_TAG, ipt_t)
#define SATFIND_CODE_GEN_ARG_OPT_T SATFIND_CODE_GEN_CONCAT(SATFIND_NAMESPACE_BASE_TAG, opt_t)
#define SATFIND_CODE_GEN_TEMPLATE_TYPE Type
#define SATFIND_CODE_GEN_TARGET_OBJ_NAME SATFIND_CODE_GEN_CONCAT(SATFIND_NAMESPACE_BASE_TAG, obj_name)
#define SATFIND_CODE_GEN_ARG_EXPAND( x ) x
#define SATFIND_CODE_GEN_ARG_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, NAME,...) NAME
#define SATFIND_CODE_GEN_ARG_PASTE(...) SATFIND_CODE_GEN_ARG_EXPAND(SATFIND_CODE_GEN_ARG_GET_MACRO(__VA_ARGS__, \
        SATFIND_CODE_GEN_ARG_PASTE64, \
        SATFIND_CODE_GEN_ARG_PASTE63, \
        SATFIND_CODE_GEN_ARG_PASTE62, \
        SATFIND_CODE_GEN_ARG_PASTE61, \
        SATFIND_CODE_GEN_ARG_PASTE60, \
        SATFIND_CODE_GEN_ARG_PASTE59, \
        SATFIND_CODE_GEN_ARG_PASTE58, \
        SATFIND_CODE_GEN_ARG_PASTE57, \
        SATFIND_CODE_GEN_ARG_PASTE56, \
        SATFIND_CODE_GEN_ARG_PASTE55, \
        SATFIND_CODE_GEN_ARG_PASTE54, \
        SATFIND_CODE_GEN_ARG_PASTE53, \
        SATFIND_CODE_GEN_ARG_PASTE52, \
        SATFIND_CODE_GEN_ARG_PASTE51, \
        SATFIND_CODE_GEN_ARG_PASTE50, \
        SATFIND_CODE_GEN_ARG_PASTE49, \
        SATFIND_CODE_GEN_ARG_PASTE48, \
        SATFIND_CODE_GEN_ARG_PASTE47, \
        SATFIND_CODE_GEN_ARG_PASTE46, \
        SATFIND_CODE_GEN_ARG_PASTE45, \
        SATFIND_CODE_GEN_ARG_PASTE44, \
        SATFIND_CODE_GEN_ARG_PASTE43, \
        SATFIND_CODE_GEN_ARG_PASTE42, \
        SATFIND_CODE_GEN_ARG_PASTE41, \
        SATFIND_CODE_GEN_ARG_PASTE40, \
        SATFIND_CODE_GEN_ARG_PASTE39, \
        SATFIND_CODE_GEN_ARG_PASTE38, \
        SATFIND_CODE_GEN_ARG_PASTE37, \
        SATFIND_CODE_GEN_ARG_PASTE36, \
        SATFIND_CODE_GEN_ARG_PASTE35, \
        SATFIND_CODE_GEN_ARG_PASTE34, \
        SATFIND_CODE_GEN_ARG_PASTE33, \
        SATFIND_CODE_GEN_ARG_PASTE32, \
        SATFIND_CODE_GEN_ARG_PASTE31, \
        SATFIND_CODE_GEN_ARG_PASTE30, \
        SATFIND_CODE_GEN_ARG_PASTE29, \
        SATFIND_CODE_GEN_ARG_PASTE28, \
        SATFIND_CODE_GEN_ARG_PASTE27, \
        SATFIND_CODE_GEN_ARG_PASTE26, \
        SATFIND_CODE_GEN_ARG_PASTE25, \
        SATFIND_CODE_GEN_ARG_PASTE24, \
        SATFIND_CODE_GEN_ARG_PASTE23, \
        SATFIND_CODE_GEN_ARG_PASTE22, \
        SATFIND_CODE_GEN_ARG_PASTE21, \
        SATFIND_CODE_GEN_ARG_PASTE20, \
        SATFIND_CODE_GEN_ARG_PASTE19, \
        SATFIND_CODE_GEN_ARG_PASTE18, \
        SATFIND_CODE_GEN_ARG_PASTE17, \
        SATFIND_CODE_GEN_ARG_PASTE16, \
        SATFIND_CODE_GEN_ARG_PASTE15, \
        SATFIND_CODE_GEN_ARG_PASTE14, \
        SATFIND_CODE_GEN_ARG_PASTE13, \
        SATFIND_CODE_GEN_ARG_PASTE12, \
        SATFIND_CODE_GEN_ARG_PASTE11, \
        SATFIND_CODE_GEN_ARG_PASTE10, \
        SATFIND_CODE_GEN_ARG_PASTE9, \
        SATFIND_CODE_GEN_ARG_PASTE8, \
        SATFIND_CODE_GEN_ARG_PASTE7, \
        SATFIND_CODE_GEN_ARG_PASTE6, \
        SATFIND_CODE_GEN_ARG_PASTE5, \
        SATFIND_CODE_GEN_ARG_PASTE4, \
        SATFIND_CODE_GEN_ARG_PASTE3, \
        SATFIND_CODE_GEN_ARG_PASTE2, \
        SATFIND_CODE_GEN_ARG_PASTE1)(__VA_ARGS__))
#define SATFIND_CODE_GEN_ARG_PASTE1
#define SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) operator_function(v1)
#define SATFIND_CODE_GEN_ARG_PASTE3(operator_function, v1, v2) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v2)
#define SATFIND_CODE_GEN_ARG_PASTE4(operator_function, v1, v2, v3) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE3(operator_function, v2, v3)
#define SATFIND_CODE_GEN_ARG_PASTE5(operator_function, v1, v2, v3, v4) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE4(operator_function, v2, v3, v4)
#define SATFIND_CODE_GEN_ARG_PASTE6(operator_function, v1, v2, v3, v4, v5) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE5(operator_function, v2, v3, v4, v5)
#define SATFIND_CODE_GEN_ARG_PASTE7(operator_function, v1, v2, v3, v4, v5, v6) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE6(operator_function, v2, v3, v4, v5, v6)
#define SATFIND_CODE_GEN_ARG_PASTE8(operator_function, v1, v2, v3, v4, v5, v6, v7) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE7(operator_function, v2, v3, v4, v5, v6, v7)
#define SATFIND_CODE_GEN_ARG_PASTE9(operator_function, v1, v2, v3, v4, v5, v6, v7, v8) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE8(operator_function, v2, v3, v4, v5, v6, v7, v8)
#define SATFIND_CODE_GEN_ARG_PASTE10(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE9(operator_function, v2, v3, v4, v5, v6, v7, v8, v9)
#define SATFIND_CODE_GEN_ARG_PASTE11(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE10(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10)
#define SATFIND_CODE_GEN_ARG_PASTE12(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE11(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)
#define SATFIND_CODE_GEN_ARG_PASTE13(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE12(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)
#define SATFIND_CODE_GEN_ARG_PASTE14(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE13(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)
#define SATFIND_CODE_GEN_ARG_PASTE15(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE14(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)
#define SATFIND_CODE_GEN_ARG_PASTE16(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE15(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15)
#define SATFIND_CODE_GEN_ARG_PASTE17(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE16(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16)
#define SATFIND_CODE_GEN_ARG_PASTE18(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE17(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17)
#define SATFIND_CODE_GEN_ARG_PASTE19(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE18(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18)
#define SATFIND_CODE_GEN_ARG_PASTE20(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE19(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19)
#define SATFIND_CODE_GEN_ARG_PASTE21(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE20(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20)
#define SATFIND_CODE_GEN_ARG_PASTE22(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE21(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21)
#define SATFIND_CODE_GEN_ARG_PASTE23(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE22(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22)
#define SATFIND_CODE_GEN_ARG_PASTE24(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE23(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23)
#define SATFIND_CODE_GEN_ARG_PASTE25(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE24(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24)
#define SATFIND_CODE_GEN_ARG_PASTE26(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE25(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25)
#define SATFIND_CODE_GEN_ARG_PASTE27(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE26(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26)
#define SATFIND_CODE_GEN_ARG_PASTE28(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE27(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27)
#define SATFIND_CODE_GEN_ARG_PASTE29(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE28(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28)
#define SATFIND_CODE_GEN_ARG_PASTE30(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE29(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29)
#define SATFIND_CODE_GEN_ARG_PASTE31(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE30(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30)
#define SATFIND_CODE_GEN_ARG_PASTE32(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE31(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31)
#define SATFIND_CODE_GEN_ARG_PASTE33(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE32(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32)
#define SATFIND_CODE_GEN_ARG_PASTE34(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE33(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33)
#define SATFIND_CODE_GEN_ARG_PASTE35(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE34(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34)
#define SATFIND_CODE_GEN_ARG_PASTE36(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE35(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35)
#define SATFIND_CODE_GEN_ARG_PASTE37(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE36(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36)
#define SATFIND_CODE_GEN_ARG_PASTE38(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE37(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37)
#define SATFIND_CODE_GEN_ARG_PASTE39(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE38(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38)
#define SATFIND_CODE_GEN_ARG_PASTE40(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE39(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39)
#define SATFIND_CODE_GEN_ARG_PASTE41(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE40(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40)
#define SATFIND_CODE_GEN_ARG_PASTE42(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE41(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41)
#define SATFIND_CODE_GEN_ARG_PASTE43(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE42(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42)
#define SATFIND_CODE_GEN_ARG_PASTE44(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE43(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43)
#define SATFIND_CODE_GEN_ARG_PASTE45(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE44(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44)
#define SATFIND_CODE_GEN_ARG_PASTE46(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE45(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45)
#define SATFIND_CODE_GEN_ARG_PASTE47(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE46(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46)
#define SATFIND_CODE_GEN_ARG_PASTE48(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE47(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47)
#define SATFIND_CODE_GEN_ARG_PASTE49(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE48(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48)
#define SATFIND_CODE_GEN_ARG_PASTE50(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE49(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49)
#define SATFIND_CODE_GEN_ARG_PASTE51(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE50(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50)
#define SATFIND_CODE_GEN_ARG_PASTE52(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE51(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51)
#define SATFIND_CODE_GEN_ARG_PASTE53(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE52(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52)
#define SATFIND_CODE_GEN_ARG_PASTE54(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE53(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53)
#define SATFIND_CODE_GEN_ARG_PASTE55(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE54(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54)
#define SATFIND_CODE_GEN_ARG_PASTE56(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE55(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55)
#define SATFIND_CODE_GEN_ARG_PASTE57(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE56(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56)
#define SATFIND_CODE_GEN_ARG_PASTE58(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE57(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57)
#define SATFIND_CODE_GEN_ARG_PASTE59(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE58(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58)
#define SATFIND_CODE_GEN_ARG_PASTE60(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE59(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59)
#define SATFIND_CODE_GEN_ARG_PASTE61(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE60(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60)
#define SATFIND_CODE_GEN_ARG_PASTE62(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE61(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61)
#define SATFIND_CODE_GEN_ARG_PASTE63(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE62(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62)
#define SATFIND_CODE_GEN_ARG_PASTE64(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63) SATFIND_CODE_GEN_ARG_PASTE2(operator_function, v1) SATFIND_CODE_GEN_ARG_PASTE63(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63)

// clang-format on