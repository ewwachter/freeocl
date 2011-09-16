#include "builtin.h"
#include "parser.h"
#include "native_type.h"
#include "symbol_table.h"
#include "overloaded_builtin.h"
#include "var.h"

namespace FreeOCL
{
	Builtin::Builtin(const smartptr<Type> &return_type, const std::string &name, const size_t num_params)
		: return_type(return_type),
		name(name),
		num_params(num_params)
	{
	}

	Builtin::~Builtin()
	{
	}

	smartptr<Type> Builtin::getReturnType(const std::deque<smartptr<Type> > &/*arg_types*/) const
	{
		return return_type;
	}

	const std::string &Builtin::getName() const
	{
		return name;
	}

	size_t Builtin::getNumParams() const
	{
		return num_params;
	}

	void Builtin::write(std::ostream& out) const
	{
		out << name << ' ';
	}

	void Parser::register_builtin()
	{
		const int types[] = { NativeType::UCHAR,	NativeType::UCHAR2,		NativeType::UCHAR3,		NativeType::UCHAR4,		NativeType::UCHAR8,		NativeType::UCHAR16,
							  NativeType::USHORT,	NativeType::USHORT2,	NativeType::USHORT3,	NativeType::USHORT4,	NativeType::USHORT8,	NativeType::USHORT16,
							  NativeType::UINT,		NativeType::UINT2,		NativeType::UINT3,		NativeType::UINT4,		NativeType::UINT8,		NativeType::UINT16,
							  NativeType::ULONG,	NativeType::ULONG2,		NativeType::ULONG3,		NativeType::ULONG4,		NativeType::ULONG8,		NativeType::ULONG16,
							  NativeType::CHAR,		NativeType::CHAR2,		NativeType::CHAR3,		NativeType::CHAR4,		NativeType::CHAR8,		NativeType::CHAR16,
							  NativeType::SHORT,	NativeType::SHORT2,		NativeType::SHORT3,		NativeType::SHORT4,		NativeType::SHORT8,		NativeType::SHORT16,
							  NativeType::INT,		NativeType::INT2,		NativeType::INT3,		NativeType::INT4,		NativeType::INT8,		NativeType::INT16,
							  NativeType::LONG,		NativeType::LONG2,		NativeType::LONG3,		NativeType::LONG4,		NativeType::LONG8,		NativeType::LONG16,
							  NativeType::FLOAT,	NativeType::FLOAT2,		NativeType::FLOAT3,		NativeType::FLOAT4,		NativeType::FLOAT8,		NativeType::FLOAT16,
							  NativeType::DOUBLE,	NativeType::DOUBLE2,	NativeType::DOUBLE3,	NativeType::DOUBLE4,	NativeType::DOUBLE8,	NativeType::DOUBLE16 };
		std::deque<int> gentype_all;
		std::deque<int> gentype_scalars;
		std::deque<int> gentype_vectors;
		std::deque<int> gentype_half;
		std::deque<int> gentype_floats;
		std::deque<int> gentype_doubles;
		std::deque<int> gentype_signed;
		std::deque<int> gentype_unsigned;
		std::deque<int> gentype_integers;
		std::deque<int> gentype_single;
		std::deque<int> gentype_vec1;
		std::deque<int> gentype_vec2;
		std::deque<int> gentype_vec3;
		std::deque<int> gentype_vec4;
		std::deque<int> gentype_vec8;
		std::deque<int> gentype_vec16;
		gentype_all.insert(gentype_all.end(), &(types[0]), &(types[sizeof(types)/sizeof(int)]));
		gentype_floats.insert(gentype_floats.end(), &(types[6 * 8]), &(types[6 * 9]));
		gentype_doubles.insert(gentype_doubles.end(), &(types[6 * 9]), &(types[6 * 10]));
		gentype_signed.insert(gentype_signed.end(), &(types[6 * 4]), &(types[6 * 8]));
		gentype_unsigned.insert(gentype_unsigned.end(), &(types[0]), &(types[6 * 4]));
		for(size_t i = 0 ; i < 10 ; ++i)
		{
			gentype_scalars.push_back(types[i * 6]);
			gentype_vectors.insert(gentype_vectors.end(), &(types[i * 6 + 1]), &(types[i * 6 + 6]));
			gentype_vec1.push_back(types[i * 6]);
			gentype_vec2.push_back(types[i * 6 + 1]);
			gentype_vec3.push_back(types[i * 6 + 2]);
			gentype_vec4.push_back(types[i * 6 + 3]);
			gentype_vec8.push_back(types[i * 6 + 4]);
			gentype_vec16.push_back(types[i * 6 + 5]);
		}
		gentype_single.push_back(0);
		gentype_integers.insert(gentype_integers.end(), gentype_signed.begin(), gentype_signed.end());
		gentype_integers.insert(gentype_integers.end(), gentype_unsigned.begin(), gentype_unsigned.end());
		gentype_half.push_back(NativeType::HALF);
		gentype_half.push_back(NativeType::HALF2);
		gentype_half.push_back(NativeType::HALF3);
		gentype_half.push_back(NativeType::HALF4);
		gentype_half.push_back(NativeType::HALF8);
		gentype_half.push_back(NativeType::HALF16);

#define REGISTER(type, name, num)					symbols->insert(#name, new Builtin(NativeType::t_##type, #name, num))
#define REGISTER_OVERLOADED(signature, gentype)		do { OverloadedBuiltin *n = new OverloadedBuiltin(signature, gentype);	symbols->insert(n->getName(), n);	} while(false)
#define REGISTER_VAR(type, name)					symbols->insert(#name, new Var(#name, NativeType::t_##type))

		// Workitem functions
		REGISTER(uint, get_work_dim, 0);
		REGISTER(size_t, get_global_size, 1);
		REGISTER(size_t, get_global_id, 1);
		REGISTER(size_t, get_local_size, 1);
		REGISTER(size_t, get_local_id, 1);
		REGISTER(size_t, get_num_groups, 1);
		REGISTER(size_t, get_group_id, 1);
		REGISTER(size_t, get_global_offset, 1);

		// Math functions
		REGISTER_OVERLOADED("gentype acos(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype acosh(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype acospi(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype asin(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype asinh(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype asinpi(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype atan(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype atan2(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype atanh(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype atanpi(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype atan2pi(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype cbrt(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype ceil(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype copysign(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype cos(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype cosh(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype cospi(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype erf(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype erfc(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype exp(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype exp2(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype exp10(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype expm1(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype fabs(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype fdim(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype floor(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype fma(gentype,gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype fmax(gentype,gentype)|gentype fmax(gentype,float)", gentype_floats);
		REGISTER_OVERLOADED("gentype fmin(gentype,gentype)|gentype fmin(gentype,float)", gentype_floats);
		REGISTER_OVERLOADED("gentype fmod(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype fract(gentype,__global gentype*)|gentype fract(gentype,__local gentype*)|gentype fract(gentype,__private gentype*)", gentype_floats);
		REGISTER_OVERLOADED("gentype frexp(gentype,__global intn*)|gentype frexp(gentype,__local intn*)|gentype frexp(gentype,__private intn*)", gentype_floats);
		REGISTER_OVERLOADED("gentype hypot(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("intn ilogb(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype ldexp(gentype,intn)|gentype ldexp(gentype,int)", gentype_floats);
		REGISTER_OVERLOADED("gentype lgamma(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype lgamma_r(gentype,__global intn*)|gentype lgamma_r(gentype,__local intn*)|gentype lgamma_r(gentype,__private intn*)", gentype_floats);
		REGISTER_OVERLOADED("gentype log(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype log2(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype log10(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype log1p(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype logb(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype mad(gentype,gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype maxmag(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype minmag(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype modf(gentype,__global gentype*)|gentype modf(gentype,__local gentype*)|gentype modf(gentype,__private gentype*)", gentype_floats);
		REGISTER_OVERLOADED("floatn nan(uintn)", gentype_floats);
		REGISTER_OVERLOADED("gentype nextafter(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype pow(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype pown(gentype,intn)", gentype_floats);
		REGISTER_OVERLOADED("gentype powr(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype remainder(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype remquo(gentype,gentype,__global intn*)|gentype remquo(gentype,gentype,__local intn*)|gentype remquo(gentype,gentype,__private intn*)", gentype_floats);
		REGISTER_OVERLOADED("gentype rint(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype rootn(gentype,intn)", gentype_floats);
		REGISTER_OVERLOADED("gentype round(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype rsqrt(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype sin(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype sincos(gentype,__global gentype*)|gentype sincos(gentype,__local gentype*)|gentype sincos(gentype,__private gentype*)", gentype_floats);
		REGISTER_OVERLOADED("gentype sinh(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype sinpi(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype sqrt(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype tan(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype tanh(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype tanpi(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype tgamma(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype trunc(gentype)", gentype_floats);
		// half_ versions
		REGISTER_OVERLOADED("gentype half_cos(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_divide(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_exp(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_exp2(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_exp10(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_log(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_log2(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_log10(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_powr(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_recip(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_rsqrt(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_sin(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_sqrt(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype half_tan(gentype)", gentype_floats);
		// native_ versions
		REGISTER_OVERLOADED("gentype native_cos(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_divide(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_exp(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_exp2(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_exp10(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_log(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_log2(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_log10(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_powr(gentype,gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_recip(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_rsqrt(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_sin(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_sqrt(gentype)", gentype_floats);
		REGISTER_OVERLOADED("gentype native_tan(gentype)", gentype_floats);

		// Integer functions
		REGISTER_OVERLOADED("ugentype abs(gentype)", gentype_integers);
		REGISTER_OVERLOADED("ugentype abs_diff(gentype,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype add_sat(gentype,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype hadd(gentype,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype rhadd(gentype,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype clamp(gentype,gentype,gentype)|gentype clamp(gentype,sgentype,sgentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype clz(gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype mad_hi(gentype,gentype,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype mad_sat(gentype,gentype,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype max(gentype,gentype)|gentype max(gentype,sgentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype min(gentype,gentype)|gentype min(gentype,sgentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype mul_hi(gentype,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype rotate(gentype,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype sub_sat(gentype,gentype)", gentype_integers);
		REGISTER_OVERLOADED("short upsample(char,uchar)"
							"|ushort upsample(uchar,uchar)"
							"|shortn upsample(charn,ucharn)"
							"|ushortn upsample(ucharn,ucharn)"
							"|int upsample(short,ushort)"
							"|uint upsample(ushort,ushort)"
							"|intn upsample(shortn,ushortn)"
							"|uintn upsample(ushortn,ushortn)"
							"|long upsample(int,uint)"
							"|ulong upsample(uint,uint)"
							"|longn upsample(intn,uintn)"
							"|ulongn upsample(uintn,uintn)", gentype_single);
		REGISTER_OVERLOADED("gentype mad24(gentype,gentype,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype mul24(gentype,gentype,gentype)", gentype_integers);

		// Common functions
		REGISTER_OVERLOADED("gentype clamp(gentype,gentype,gentype)|gentype clamp(gentype,float,float)", gentype_floats);
		REGISTER_OVERLOADED("gentype degrees(gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype max(gentype,gentype)|gentype max(gentype,float)", gentype_integers);
		REGISTER_OVERLOADED("gentype min(gentype,gentype)|gentype min(gentype,float)", gentype_integers);
		REGISTER_OVERLOADED("gentype mix(gentype,gentype,gentype)|gentype mix(gentype,gentype,float)", gentype_floats);
		REGISTER_OVERLOADED("gentype radians(gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype step(gentype,gentype)|gentype step(float,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype smoothstep(gentype,gentype,gentype)|gentype step(float,float,gentype)", gentype_integers);
		REGISTER_OVERLOADED("gentype sign(gentype)", gentype_integers);

		// Geometric functions
		REGISTER_OVERLOADED("float4 cross(float4,float4)|float3 cross(float3,float3)", gentype_single);
		REGISTER_OVERLOADED("float dot(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("float distance(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("float length(floatn)", gentype_floats);
		REGISTER_OVERLOADED("floatn normalize(floatn)", gentype_floats);
		REGISTER_OVERLOADED("float fast_distance(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("float fast_length(floatn)", gentype_floats);
		REGISTER_OVERLOADED("floatn fast_normalize(floatn)", gentype_floats);

		// Relational functions
		REGISTER_OVERLOADED("intn isequal(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn isnotequal(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn isgreater(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn isgreaterequal(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn isless(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn islessequal(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn islessgreater(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn isfinite(floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn isinf(floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn isnan(floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn isnormal(floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn isordered(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn isunordered(floatn,floatn)", gentype_floats);
		REGISTER_OVERLOADED("intn signbit(floatn)", gentype_floats);
		REGISTER_OVERLOADED("int any(gentype)", gentype_signed);
		REGISTER_OVERLOADED("int all(gentype)", gentype_signed);
		REGISTER_OVERLOADED("gentype bitselect(gentype,gentype,gentype)", gentype_all);
		REGISTER_OVERLOADED("gentype select(gentype,gentype,ugentype)|gentype select(gentype,gentype,igentype)", gentype_all);

		// Sync functions
		REGISTER_VAR(int, CLK_LOCAL_MEM_FENCE);
		REGISTER_VAR(int, CLK_GLOBAL_MEM_FENCE);
		REGISTER(void, barrier, 1);

		// Memory fence function
		REGISTER(void, mem_fence, 1);
		REGISTER(void, read_mem_fence, 1);
		REGISTER(void, write_mem_fence, 1);

		// Async Copies from Global to Local Memory, Local to Global Memory, and Prefetch
		// TODO

		// Atomic Functions
		REGISTER_OVERLOADED("int atomic_add(__global int*, int)|uint atomic_add(__global uint*, uint)|int atomic_add(__local int*, int)|uint atomic_add(__local uint*, uint)", gentype_single);
		REGISTER_OVERLOADED("int atomic_sub(__global int*, int)|uint atomic_sub(__global uint*, uint)|int atomic_sub(__local int*, int)|uint atomic_sub(__local uint*, uint)", gentype_single);
		REGISTER_OVERLOADED("int atomic_xchg(__global int*, int)"
							"|uint atomic_xchg(__global uint*, uint)"
							"|float atomic_xchg(__global float*, float)"
							"|int atomic_xchg(__local int*, int)"
							"|uint atomic_xchg(__local uint*, uint)"
							"|float atomic_xchg(__local float*, float)", gentype_single);
		REGISTER_OVERLOADED("int atomic_inc(__global int*)|uint atomic_inc(__global uint*)|int atomic_inc(__local int*)|uint atomic_inc(__local uint*)", gentype_single);
		REGISTER_OVERLOADED("int atomic_dec(__global int*)|uint atomic_dec(__global uint*)|int atomic_dec(__local int*)|uint atomic_dec(__local uint*)", gentype_single);
		REGISTER_OVERLOADED("int atomic_cmpxchg(__global int*, int, int)"
							"|uint atomic_cmpxchg(__global uint*, uint, uint)"
							"|int atomic_cmpxchg(__local int*, int, int)"
							"|uint atomic_cmpxchg(__local uint*, uint, uint)", gentype_single);
		REGISTER_OVERLOADED("int atomic_min(__global int*, int)|uint atomic_min(__global uint*, uint)|int atomic_min(__local int*, int)|uint atomic_min(__local uint*, uint)", gentype_single);
		REGISTER_OVERLOADED("int atomic_max(__global int*, int)|uint atomic_max(__global uint*, uint)|int atomic_max(__local int*, int)|uint atomic_max(__local uint*, uint)", gentype_single);
		REGISTER_OVERLOADED("int atomic_and(__global int*, int)|uint atomic_and(__global uint*, uint)|int atomic_and(__local int*, int)|uint atomic_and(__local uint*, uint)", gentype_single);
		REGISTER_OVERLOADED("int atomic_or(__global int*, int)|uint atomic_or(__global uint*, uint)|int atomic_or(__local int*, int)|uint atomic_or(__local uint*, uint)", gentype_single);
		REGISTER_OVERLOADED("int atomic_xor(__global int*, int)|uint atomic_xor(__global uint*, uint)|int atomic_xor(__local int*, int)|uint atomic_xor(__local uint*, uint)", gentype_single);

		// Miscellaneous Vector Functions
		REGISTER_OVERLOADED("int vec_step(gentype)", gentype_all);
		REGISTER_OVERLOADED("int vec_step(gentype)", gentype_half);
//		REGISTER_OVERLOADED("int shuffle(gentype)", gentype_half);
//		REGISTER_OVERLOADED("int shuffle2(gentype)", gentype_half);

		// Conversion functions
		REGISTER_OVERLOADED("char convert_char(gentype)", gentype_vec1);
		REGISTER_OVERLOADED("uchar convert_uchar(gentype)", gentype_vec1);
		REGISTER_OVERLOADED("short convert_short(gentype)", gentype_vec1);
		REGISTER_OVERLOADED("ushort convert_ushort(gentype)", gentype_vec1);
		REGISTER_OVERLOADED("int convert_int(gentype)", gentype_vec1);
		REGISTER_OVERLOADED("uint convert_uint(gentype)", gentype_vec1);
		REGISTER_OVERLOADED("long convert_long(gentype)", gentype_vec1);
		REGISTER_OVERLOADED("ulong convert_ulong(gentype)", gentype_vec1);
		REGISTER_OVERLOADED("float convert_float(gentype)", gentype_vec1);
		REGISTER_OVERLOADED("double convert_double(gentype)", gentype_vec1);

		REGISTER_OVERLOADED("char2 convert_char2(gentype)", gentype_vec2);
		REGISTER_OVERLOADED("uchar2 convert_uchar2(gentype)", gentype_vec2);
		REGISTER_OVERLOADED("short2 convert_short2(gentype)", gentype_vec2);
		REGISTER_OVERLOADED("ushort2 convert_ushort2(gentype)", gentype_vec2);
		REGISTER_OVERLOADED("int2 convert_int2(gentype)", gentype_vec2);
		REGISTER_OVERLOADED("uint2 convert_uint2(gentype)", gentype_vec2);
		REGISTER_OVERLOADED("long2 convert_long2(gentype)", gentype_vec2);
		REGISTER_OVERLOADED("ulong2 convert_ulong2(gentype)", gentype_vec2);
		REGISTER_OVERLOADED("float2 convert_float2(gentype)", gentype_vec2);
		REGISTER_OVERLOADED("double2 convert_double2(gentype)", gentype_vec2);

		REGISTER_OVERLOADED("char3 convert_char3(gentype)", gentype_vec3);
		REGISTER_OVERLOADED("uchar3 convert_uchar3(gentype)", gentype_vec3);
		REGISTER_OVERLOADED("short3 convert_short3(gentype)", gentype_vec3);
		REGISTER_OVERLOADED("ushort3 convert_ushort3(gentype)", gentype_vec3);
		REGISTER_OVERLOADED("int3 convert_int3(gentype)", gentype_vec3);
		REGISTER_OVERLOADED("uint3 convert_uint3(gentype)", gentype_vec3);
		REGISTER_OVERLOADED("long3 convert_long3(gentype)", gentype_vec3);
		REGISTER_OVERLOADED("ulong3 convert_ulong3(gentype)", gentype_vec3);
		REGISTER_OVERLOADED("float3 convert_float3(gentype)", gentype_vec3);
		REGISTER_OVERLOADED("double3 convert_double3(gentype)", gentype_vec3);

		REGISTER_OVERLOADED("char4 convert_char4(gentype)", gentype_vec4);
		REGISTER_OVERLOADED("uchar4 convert_uchar4(gentype)", gentype_vec4);
		REGISTER_OVERLOADED("short4 convert_short4(gentype)", gentype_vec4);
		REGISTER_OVERLOADED("ushort4 convert_ushort4(gentype)", gentype_vec4);
		REGISTER_OVERLOADED("int4 convert_int4(gentype)", gentype_vec4);
		REGISTER_OVERLOADED("uint4 convert_uint4(gentype)", gentype_vec4);
		REGISTER_OVERLOADED("long4 convert_long4(gentype)", gentype_vec4);
		REGISTER_OVERLOADED("ulong4 convert_ulong4(gentype)", gentype_vec4);
		REGISTER_OVERLOADED("float4 convert_float4(gentype)", gentype_vec4);
		REGISTER_OVERLOADED("double4 convert_double4(gentype)", gentype_vec4);

		REGISTER_OVERLOADED("char8 convert_char8(gentype)", gentype_vec8);
		REGISTER_OVERLOADED("uchar8 convert_uchar8(gentype)", gentype_vec8);
		REGISTER_OVERLOADED("short8 convert_short8(gentype)", gentype_vec8);
		REGISTER_OVERLOADED("ushort8 convert_ushort8(gentype)", gentype_vec8);
		REGISTER_OVERLOADED("int8 convert_int8(gentype)", gentype_vec8);
		REGISTER_OVERLOADED("uint8 convert_uint8(gentype)", gentype_vec8);
		REGISTER_OVERLOADED("long8 convert_long8(gentype)", gentype_vec8);
		REGISTER_OVERLOADED("ulong8 convert_ulong8(gentype)", gentype_vec8);
		REGISTER_OVERLOADED("float8 convert_float8(gentype)", gentype_vec8);
		REGISTER_OVERLOADED("double8 convert_double8(gentype)", gentype_vec8);

		REGISTER_OVERLOADED("char16 convert_char16(gentype)", gentype_vec16);
		REGISTER_OVERLOADED("uchar16 convert_uchar16(gentype)", gentype_vec16);
		REGISTER_OVERLOADED("short16 convert_short16(gentype)", gentype_vec16);
		REGISTER_OVERLOADED("ushort16 convert_ushort16(gentype)", gentype_vec16);
		REGISTER_OVERLOADED("int16 convert_int16(gentype)", gentype_vec16);
		REGISTER_OVERLOADED("uint16 convert_uint16(gentype)", gentype_vec16);
		REGISTER_OVERLOADED("long16 convert_long16(gentype)", gentype_vec16);
		REGISTER_OVERLOADED("ulong16 convert_ulong16(gentype)", gentype_vec16);
		REGISTER_OVERLOADED("float16 convert_float16(gentype)", gentype_vec16);
		REGISTER_OVERLOADED("double16 convert_double16(gentype)", gentype_vec16);

#undef REGISTER
#undef REGISTER_OVERLOADED
#undef REGISTER_VAR
	}
}