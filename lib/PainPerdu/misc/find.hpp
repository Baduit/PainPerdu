#pragma once

#include <concepts>
#include <string_view>
#include <ranges>

#include <heart/misc/Id.hpp>

namespace heart
{

namespace misc
{

/*
** Concepts on elements
*/
template <typename T, typename NameType = std::string_view>
concept HasName = 
	requires(const T& t)
	{
		{ t.name } -> std::equality_comparable_with<NameType>;
	};

template <typename T, typename NameType = std::string_view>
concept HasTypeName = 
	requires(const T& t)
	{
		{ t.type_name } -> std::equality_comparable_with<NameType>;
	};

template <typename T, typename IdType = Id>
concept HasId = 
	requires(const T& t)
	{
		{ t.id } -> std::equality_comparable_with<IdType>;
	};

/*
** Concepts on ranges
*/
template <typename Range>
concept RangeWithNames =
	std::ranges::range<Range> &&
	HasName<std::ranges::range_value_t<Range>, std::string_view>;

template <typename Range>
concept RangeWithTypeNames =
	std::ranges::range<Range> &&
	HasTypeName<std::ranges::range_value_t<Range>, std::string_view>;

template <typename Range>
concept RangeWithIds =
	std::ranges::range<Range> &&
	HasId<std::ranges::range_value_t<Range>, Id>;

/*
** Functions
*/
template <RangeWithNames Range>
[[nodiscard]] auto find_by_name(Range&& r, std::string_view name)
{
	auto it = std::find_if(std::begin(r), std::end(r),
		[&](const auto& v)
		{
			return v.name == name;
		});
	return it;
}

template <RangeWithTypeNames Range>
[[nodiscard]] auto find_by_type_name(Range&& r, std::string_view type_name)
{
	auto it = std::find_if(std::begin(r), std::end(r),
		[&](const auto& v)
		{
			return v.type_name == type_name;
		});
	return it;
}

template <RangeWithIds Range>
[[nodiscard]] auto find_by_id(Range&& r, Id id)
{
	auto it = std::find_if(std::begin(r), std::end(r),
		[&](const auto& v)
		{
			return v.id == id;
		});
	return it;
}

} // namespace misc


} // namespace heart
