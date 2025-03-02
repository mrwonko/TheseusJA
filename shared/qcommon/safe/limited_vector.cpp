#include "limited_vector.h"

// We want to perform some validations for LimitedVector, and we need some test types for that.
// We hide them in this anonymous namespace, and expect the optimizer to omit them from the final binary,
// as they are only used in static_asserts at compile time.
namespace
{
	struct NonTriviallyDestructible
	{
		~NonTriviallyDestructible() {}
	};
	struct MoveOnly
	{
		MoveOnly() = default;
		MoveOnly(const MoveOnly&) = delete;
		MoveOnly& operator=(const MoveOnly&) = delete;
		MoveOnly(MoveOnly&&) = default;
		MoveOnly& operator=(MoveOnly&&) = default;
	};
	struct Immutable
	{
		Immutable() = default;
		Immutable(const Immutable&) = default;
		Immutable& operator=(const Immutable&) = delete;
		Immutable(Immutable&&) = delete;
		Immutable& operator=(Immutable&&) = delete;
	};

	// Validate that whether a LimitedVector is trivially destructible depends on whether its elements are.
	static_assert(!std::is_trivially_constructible < NonTriviallyDestructible >::value, "NonTriviallyDestructible is a test class that should not be trivially destructible");
	static_assert(std::is_trivially_destructible< Q::LimitedVector< int, 69 > >::value, "LimitedVectors of trivially destructible types are themselves trivially destructible.");
	static_assert(!std::is_trivially_destructible< Q::LimitedVector< NonTriviallyDestructible, 420 > >::value, "LimitedVectors of non-trivially destructible types are themselves non-trivially destructible.");

	// Likewise, (non)copyability is inherited from the elements.
	static_assert(std::is_copy_constructible< Q::LimitedVector< int, 69 > >::value, "LimitedVectors of copyable types are themselves copyable.");
	static_assert(!std::is_copy_constructible< Q::LimitedVector< MoveOnly, 420 > >::value, "LimitedVectors of non-copyable types are themselves non-copyable.");
	static_assert(std::is_copy_assignable< Q::LimitedVector< int, 69 > >::value, "LimitedVectors of copy-assignable types are themselves copy-assignable.");
	static_assert(!std::is_copy_assignable< Q::LimitedVector< MoveOnly, 420 > >::value, "LimitedVectors of non-copy-assignable types are themselves non-copy-assignable.");

	// Moveability:
	static_assert(std::is_move_constructible< Q::LimitedVector< MoveOnly, 69 > >::value, "LimitedVectors of movable types are themselves movable.");
	static_assert(!std::is_move_constructible< Q::LimitedVector< Immutable, 69 > >::value, "LimitedVectors of immovable types are themselves immovable.");
	static_assert(std::is_move_assignable< Q::LimitedVector< MoveOnly, 69 > >::value, "LimitedVectors of movable types are themselves move-assignable.");
	static_assert(!std::is_move_assignable< Q::LimitedVector< Immutable, 69 > >::value, "LimitedVectors of immovable types are themselves not move-assignable.");
}
