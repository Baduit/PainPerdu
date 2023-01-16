#pragma once

#include <tao/pegtl/contrib/parse_tree.hpp>

#include <PainPerdu/Definitions.hpp>
#include <PainPerdu/parser/Grammar.hpp>

namespace PainPerdu::parser
{

struct TreeToDefVisitor
{
	TreeToDefVisitor(Definitions& defs):
		_defs(defs)
	{}

	template <typename InstructionN, typename InstructionId>
	Instruction do_it_right(const pegtl::parse_tree::node& node)
	{
		const auto& child = *(node.children[0]);
		if (child.is_type<Integer>())
		{
			decltype(std::declval<InstructionN>().value) n;
			std::from_chars(child.string_view().data(), child.string_view().data() + child.string_view().size(), n);

			return InstructionN{ n };
		}
		else
		{
			return InstructionId{ child.string() };
		}
	}

	template <typename InstructionAlone, typename InstructionN, typename InstructionId>
	Instruction do_it_right(const pegtl::parse_tree::node& node)
	{
		if (node.children.empty())
		{
			return InstructionAlone{};
		}
		else
		{
			const auto& child = *(node.children[0]);
			if (child.is_type<Integer>())
			{
				decltype(std::declval<InstructionN>().value) n;
				std::from_chars(child.string_view().data(), child.string_view().data() + child.string_view().size(), n);
				return InstructionN{ n };
			}
			else
			{
				return InstructionId{ child.string() };
			}
		}
	}

	void operator()(const pegtl::parse_tree::node* node)
	{
		if (!node)
			return;			

		if (node->is_type<DefineLabel>())
		{
			_defs.annotations.push_back(annotations::DefineLabel{ node->children[0]->string(), _defs.recipe.size() });
		}
		else if (node->is_type<MoveRight>())
		{
			_defs.recipe.push_back(do_it_right<instructions::MoveRight, instructions::MoveRightRef>(*node));
		}
		else if (node->is_type<MoveLeft>())
		{
			_defs.recipe.push_back(do_it_right<instructions::MoveLeft, instructions::MoveLeftRef>(*node));
		}
		else if (node->is_type<Increment>())
		{
			_defs.recipe.push_back(do_it_right<instructions::Increment, instructions::IncrementRef>(*node));
		}
		else if (node->is_type<Decrement>())
		{
			_defs.recipe.push_back(do_it_right<instructions::Decrement, instructions::DecrementRef>(*node));
		}
		else if (node->is_type<ResetCase>())
		{
			_defs.recipe.push_back(instructions::ResetCase{});
		}
		else if (node->is_type<DefineReference>())
		{
			_defs.recipe.push_back(instructions::DefineReference{ node->children[0]->string() });
		}
		else if (node->is_type<UndefineReference>())
		{
			_defs.recipe.push_back(instructions::UndefineReference{ node->children[0]->string() });
		}
		else if (node->is_type<MoveToReference>())
		{
			_defs.recipe.push_back(instructions::MoveToReference{ node->children[0]->string() });
		}
		else if (node->is_type<GoToLabel>())
		{
			_defs.recipe.push_back(instructions::GoToLabel{ node->children[0]->string() });
		}
		else if (node->is_type<Rewind>())
		{
			_defs.recipe.push_back(instructions::Rewind{ node->children[0]->string() });
		}
		else if (node->is_type<IfCurrentValueDifferent>())
		{
			_defs.recipe.push_back(do_it_right<instructions::IfCurrentValueDifferent0, instructions::IfCurrentValueEqualsN, instructions::IfCurrentValueEqualsNRef>(*node));
		}
		else if (node->is_type<IfCursorIsAtReference>())
		{
			_defs.recipe.push_back(instructions::IfCursorIsAtReference{ node->children[0]->string() });
		}
		else if (node->is_type<IfReferenceExists>())
		{
			_defs.recipe.push_back(instructions::IfReferenceExists{ node->children[0]->string() });
		}
		else if (node->is_type<GetChar>())
		{
			_defs.recipe.push_back(instructions::GetChar{});
		}
		else if (node->is_type<PutChar>())
		{
			_defs.recipe.push_back(instructions::PutChar{});
		}
		else if (node->is_type<ReadFile>())
		{
			auto filename = node->string_view();
			filename.remove_prefix(1);
			filename.remove_suffix(1);
			_defs.recipe.push_back(instructions::ReadFile{ std::string(filename) });
		}
		else
		{
			for (const auto& child: node->children)
				(*this)(child.get());
		}
	}

	Definitions& _defs;
};

} // namespace PainPerdu::parser