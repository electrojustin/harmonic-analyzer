#pragma once
const map <int, string> IntervalTree{
	{ 0, "Unison" },
	{ 3, "Min. 3rd" },
	{ 4, "Maj. 3rd" },
	{ 34, "m" },
	{ 33, "dim" },
	{ 43, "M" },
	{ 44, "+" },
	{ 343, "m7" },
	{ 344, "mM7" },
	{ 333, "dim7" },
	{ 334, "m7b5" },
	{ 442, "+7" },
	{ 443, "+M7" },
	{ 433, "7" },
	{ 434, "M7" } 
};

const map <int, string> MajorExtensionTree{
	{13, " addb9"},
	{14, " add9" },
	{17, " addb11"},
	{18, " add11"},
	{20, " addb13"},
	{21, " add13"}
};

const map <int, string> MinorExtensionTree{
	{ 13, " add9" },
	{ 14, " add#9" },
	{ 17, " add11" },
	{ 18, " add#11" },
	{ 20, " add13" },
	{ 21, " add#13" }
};
