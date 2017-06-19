#include "Test.hpp"

int test(std::ostream &outStream)
{
	int failed = 0;

	/*************************************************************************
	 * HAPPY PATH TESTS
	 *************************************************************************/

	auto r1 = xg::newGuid();
	auto r2 = xg::newGuid();
	auto r3 = xg::newGuid();

	outStream << r1 << std::endl << r2 << std::endl << r3 << std::endl;

	xg::Guid s1("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
	xg::Guid s2("16d1bd03-09a5-47d3-944b-5e326fd52d27");
	xg::Guid s3("fdaba646-e07e-49de-9529-4499a5580c75");
	xg::Guid s4("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");

	if (r1 == r2 || r1 == r3 || r2 == r3)
	{
		outStream << "FAIL - not all random guids are different" << std::endl;
		failed++;
	}

	if (s1 == s2)
	{
		outStream << "FAIL - s1 and s2 should be different" << std::endl;
		failed++;
	}

	if (s1 != s4)
	{
		outStream << "FAIL - s1 and s4 should be equal" << std::endl;
		failed++;
	}

	std::stringstream ss1;
	ss1 << s1;
	if (ss1.str() != "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e")
	{
		outStream << "FAIL - string from s1 stream is wrong" << std::endl;
		outStream << "--> " << ss1.str() << std::endl;
		failed++;
	}

	if (s1.str() != "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e")
	{
		outStream << "FAIL - string from s1.str() is wrong" << std::endl;
		outStream << "--> " << s1.str() << std::endl;
		failed++;
	}

	std::stringstream ss2;
	ss2 << s2;
	if (ss2.str() != "16d1bd03-09a5-47d3-944b-5e326fd52d27")
	{
		outStream << "FAIL - string generated from s2 is wrong" << std::endl;
		outStream << "--> " << ss2.str() << std::endl;
		return 1;
	}

	std::stringstream ss3;
	ss3 << s3;
	if (ss3.str() != "fdaba646-e07e-49de-9529-4499a5580c75")
	{
		outStream << "FAIL - string generated from s3 is wrong" << std::endl;
		outStream << "--> " << ss3.str() << std::endl;
		failed++;
	}

	auto swap1 = xg::newGuid();
	auto swap2 = xg::newGuid();
	auto swap3 = swap1;
	auto swap4 = swap2;

	if (swap1 != swap3 || swap2 != swap4 || swap1 == swap2)
	{
		outStream << "FAIL - swap guids have bad initial state" << std::endl;
		failed++;
	}

	swap1.swap(swap2);

	if (swap1 != swap4 || swap2 != swap3 || swap1 == swap2)
	{
		outStream << "FAIL - swap didn't swap" << std::endl;
		failed++;
	}

	/*************************************************************************
	* ERROR HANDLING
	*************************************************************************/

	xg::Guid empty;
	std::vector<unsigned char> notEnoughBytes{ 1, 2, 3, 4 };
	xg::Guid tooFew(notEnoughBytes);
	if (tooFew != empty || tooFew.isValid())
	{
		outStream << "FAIL - Guid created from too few bytes" << std::endl;
		failed++;
	}

	std::vector<unsigned char> tooManyBytes
	{
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17
	};
	xg::Guid tooMany(tooManyBytes);
	if (tooMany != empty || tooMany.isValid())
	{
		outStream << "FAIL - Guid created from too many bytes" << std::endl;
		failed++;
	}

	if (failed == 0)
	{
		outStream << "All tests passed!" << std::endl;
		return 0;
	}
	else
	{
		outStream << failed << " tests failed." << std::endl;
		return 1;
	}
}
