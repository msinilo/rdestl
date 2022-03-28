SECTION("EmptyString")
{
	STRING_CLASS s;
	CHECK(s.empty());
	CHECK(0 == s.length());
}
SECTION("LenOfCString")
{
	STRING_CLASS s("alamakota");
	CHECK(!s.empty());
	CHECK(9 == s.length());
}
SECTION("CopyCtorLen")
{
	STRING_CLASS s("alamakota");
	STRING_CLASS s2(s);
	CHECK(9 == s2.length());
	CHECK('a' == s2[0]);
	CHECK('l' == s2[1]);
	CHECK('a' == s2[8]);
}
SECTION("Compare")
{
	STRING_CLASS s("alamakota");
	STRING_CLASS s2("alamakota");
	CHECK(s.compare("ala") != 0);
	CHECK(0 == s.compare(s2));
	STRING_CLASS s3("alamakot");
	CHECK(+1 == s.compare(s3));
	CHECK(-1 == s3.compare(s2));
}
SECTION("EqOp")
{
	STRING_CLASS s("alamakota");
	STRING_CLASS s2(s);
	CHECK(s == s2);
}

SECTION("AssignString")
{
	STRING_CLASS s;
	STRING_CLASS s2("ala");
	s = s2;
	CHECK('a' == s[0]);
	CHECK('l' == s[1]);
	CHECK('a' == s[2]);
	CHECK(0 == s.compare(s2));
	CHECK(0 == s.compare("ala"));
	CHECK(s == s2);
}
SECTION("AssignCString")
{
	STRING_CLASS s;
	s = "alamakota";
	CHECK(0 == s.compare("alamakota"));
	CHECK(9 == s.length());
}
SECTION("Substring")
{
	STRING_CLASS s("alamakota");
	STRING_CLASS s2(s.substr(0, 3));
	CHECK(s2.compare("ala") == 0);
}

SECTION("AppendStr")
{
	STRING_CLASS s("ala");
	STRING_CLASS scopy(s);
	STRING_CLASS s2("makota");
	s.append(s2);
	CHECK(s.compare("alamakota") == 0);
	CHECK(s2.compare("makota") == 0);
	CHECK(scopy.compare("ala") == 0);
}
SECTION("AppendCStr")
{
	STRING_CLASS s("ala");
	STRING_CLASS scopy(s);
	s.append("-ma-kota");
	CHECK(s.compare("ala-ma-kota") == 0);
}
SECTION("AppendStrNoMakeUnique")
{
	STRING_CLASS s("ala");
	STRING_CLASS s2("makota");
	s.append(s2);
	CHECK(s.compare("alamakota") == 0);
	CHECK(s2.compare("makota") == 0);
}
SECTION("AppendForceRealloc")
{
	STRING_CLASS s("ala");
	STRING_CLASS s2("makota1234567890123456789012345678901234567890");
	s.append(s2);
	CHECK(s.compare("alamakota1234567890123456789012345678901234567890") == 0);
}
SECTION("MakeLower")
{
	STRING_CLASS s("AlAMaKoTA");
	s.make_lower();
	CHECK(s.compare("alamakota") == 0);
}
SECTION("MakeUpper")
{
	STRING_CLASS s("AlAMaKoTA");
	s.make_upper();
	CHECK(s.compare("ALAMAKOTA") == 0);
}
SECTION("CopyEmpty")
{
	STRING_CLASS s;
	STRING_CLASS s2(s);
	CHECK(s2.empty());
}


//-----------------------------------------------------------------
//Test search

SECTION("Find")
{
	STRING_CLASS t("hello world rde stl is fast");

	STRING_CLASS::size_type x = t.find("hello");
	CHECK(x == 0);

	x = t.find("is");
	CHECK(x == 20);

	x = t.find("fast");
	CHECK(x == 23);

	x = t.find("st"); //start of 'stl'
	CHECK(x == 16);

	x = t.find(" ");
	CHECK(x == 5);

	x = t.find("java");
	CHECK(x == STRING_CLASS::npos);

	x = t.find("fastideous");
	CHECK(x == STRING_CLASS::npos);
}

SECTION("ReverseFind")
{
	STRING_CLASS t("hello world rde stl is fast");

	STRING_CLASS::size_type x = t.rfind("hello");
	CHECK(x == 0);

	x = t.rfind("is");
	CHECK(x == 20);

	x = t.rfind("fast");
	CHECK(x == 23);

	// these are different for rfind
	x = t.rfind("st");
	CHECK(x == 25); //end of 'fast'

	x = t.rfind(" ");
	CHECK(x == 22); //just before 'fast'

	x = t.rfind("java");
	CHECK(STRING_CLASS::npos == x);

	x = t.rfind("fastideous");
	CHECK(STRING_CLASS::npos == x);

	t = STRING_CLASS("ste stf stg sth");
	x = t.rfind("stf");
	CHECK(4 == x);
}

//-----------------------------------------------------------------
//Test string storage

SECTION("Append")
{
	STRING_CLASS s;
	s.append("hello");
	CHECK(s.compare(STRING_CLASS("hello")) == 0);

	s.append("world");
	CHECK(s.compare(STRING_CLASS("helloworld")) == 0);

	s.append("rde");
	CHECK(s.compare(STRING_CLASS("helloworldrde")) == 0);

}

SECTION("CopyStorage")
{
	STRING_CLASS t("HelloWorldThisIsALongString");
	STRING_CLASS s("HelloWorldThisIsAReallyReallyReallyReallyReallyReallyReallyReallyReallyReallyReallyReallyReallyReallyReallyReallyReallyReallyLongString");
	STRING_CLASS x(t);
	x.clear();
	x = s;
	CHECK(x.length() == s.length());
	CHECK(x.compare(s) == 0);
}
