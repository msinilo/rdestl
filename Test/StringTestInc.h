	TESTC(EmptyString)
	{
		STRING_CLASS s;
		CHECK(s.empty());
		CHECK_EQUAL(0ul, s.length());
	}
	TESTC(LenOfCString)
	{
		STRING_CLASS s("alamakota");
		CHECK(!s.empty());
		CHECK_EQUAL(9ul, s.length());
	}
	TESTC(CopyCtorLen)
	{
		STRING_CLASS s("alamakota");
		STRING_CLASS s2(s);
		CHECK_EQUAL(9ul, s2.length());
		CHECK_EQUAL('a', s2[0]);
		CHECK_EQUAL('l', s2[1]);
		CHECK_EQUAL('a', s2[8]);
	}
	TESTC(Compare)
	{
		STRING_CLASS s("alamakota");
		STRING_CLASS s2("alamakota");
		CHECK(s.compare("ala") != 0);
		CHECK_EQUAL(0, s.compare(s2));
		STRING_CLASS s3("alamakot");
		CHECK_EQUAL(+1, s.compare(s3));
		CHECK_EQUAL(-1, s3.compare(s2));
	}
	TESTC(EqOp)
	{
		STRING_CLASS s("alamakota");
		STRING_CLASS s2(s);
		CHECK(s == s2);
	}

	TESTC(AssignString)
	{
		STRING_CLASS s;
		STRING_CLASS s2("ala");
		s = s2;
		CHECK_EQUAL('a', s[0]);
		CHECK_EQUAL('l', s[1]);
		CHECK_EQUAL('a', s[2]);
		CHECK_EQUAL(0, s.compare(s2));
		CHECK_EQUAL(0, s.compare("ala"));
		CHECK(s == s2);
	}
	TESTC(AssignCString)
	{
		STRING_CLASS s;
		s = "alamakota";
		CHECK_EQUAL(0, s.compare("alamakota"));
		CHECK_EQUAL(9ul, s.length());
	}
	TESTC(Substring)
	{
		STRING_CLASS s("alamakota");
		STRING_CLASS s2(s.substr(0, 3));
		CHECK(s2.compare("ala") == 0);
	}

	TESTC(AppendStr)
	{
		STRING_CLASS s("ala");
		STRING_CLASS scopy(s);
		STRING_CLASS s2("makota");
		s.append(s2);
		CHECK(s.compare("alamakota") == 0);
		CHECK(s2.compare("makota") == 0);
		CHECK(scopy.compare("ala") == 0);
	}
	TESTC(AppendCStr)
	{
		STRING_CLASS s("ala");
		STRING_CLASS scopy(s);
		s.append("-ma-kota");
		CHECK(s.compare("ala-ma-kota") == 0);
	}
	TESTC(AppendStrNoMakeUnique)
	{
		STRING_CLASS s("ala");
		STRING_CLASS s2("makota");
		s.append(s2);
		CHECK(s.compare("alamakota") == 0);
		CHECK(s2.compare("makota") == 0);
	}
	TESTC(AppendForceRealloc)
	{
		STRING_CLASS s("ala");
		STRING_CLASS s2("makota1234567890123456789012345678901234567890");
		s.append(s2);
		CHECK(s.compare("alamakota1234567890123456789012345678901234567890") == 0);
	}
