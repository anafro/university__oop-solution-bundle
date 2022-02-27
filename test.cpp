#include <gtest/gtest.h>
#include <bigint.h++>
#include <csv.h++>
#include <dictionary.h++>
#include <stack.h++>
#include <bigint_command_executor.h++>

using namespace PROJECT_NAME;

TEST(BigInt, ZeroConstructor) {
    bigint zero;

    EXPECT_STREQ(zero.get_numeric_string().c_str(), "0");
    EXPECT_EQ(zero.get_sign(), true);
}

TEST(BigInt, NumericStringConstructor) {
    bigint positive { "291384912834912834911293481923841983491283491823" };
    bigint negative { "-34248529384502348523495923045920345204395024395" };

    EXPECT_STREQ(positive.get_numeric_string().c_str(), "291384912834912834911293481923841983491283491823");
    EXPECT_EQ(positive.get_sign(), true);

    EXPECT_STREQ(negative.get_numeric_string().c_str(), "34248529384502348523495923045920345204395024395");
    EXPECT_EQ(negative.get_sign(), false);
}

TEST(BigInt, NumericStringConstructorBadInput) {
    EXPECT_THROW(bigint {"-adfjakfald"}, std::invalid_argument);
    EXPECT_THROW(bigint {"adfjakfald"}, std::invalid_argument);
    EXPECT_THROW(bigint {" 912834918234912839"}, std::invalid_argument);
    EXPECT_THROW(bigint {"912834918234912839\n"}, std::invalid_argument);
}

TEST(BigInt, IntegerConstructor) {
    bigint negative = -42839149, positive = 394283949;
    EXPECT_STREQ(positive.get_numeric_string().c_str(), "394283949");
    EXPECT_EQ(positive.get_sign(), true);

    EXPECT_STREQ(negative.get_numeric_string().c_str(), "42839149");
    EXPECT_EQ(negative.get_sign(), false);
}

TEST(BigInt, SetValue) {
    bigint integer;
    integer.set_value("-4923849127349127349127349123");

    EXPECT_STREQ(integer.get_numeric_string().c_str(), "4923849127349127349127349123");
    EXPECT_EQ(integer.get_sign(), false);
}

TEST(BigInt, SetSignBool) {
    bigint integer("9582349582394582934859");
    integer.set_sign(false);

    EXPECT_STREQ(integer.get_numeric_string().c_str(), "9582349582394582934859");
    EXPECT_EQ(integer.get_sign(), false);

    integer.set_sign(true);

    EXPECT_EQ(integer.get_sign(), true);
}

TEST(BigInt, SetSignChar) {
    bigint integer("9582349582394582934859");
    integer.set_sign('+');

    EXPECT_STREQ(integer.get_numeric_string().c_str(), "9582349582394582934859");
    EXPECT_EQ(integer.get_sign(), true);

    integer.set_sign('-');

    EXPECT_EQ(integer.get_sign(), false);
}

TEST(BigInt, SetSignCharInvalid) {
    bigint integer("9582349582394582934859");
    EXPECT_THROW(integer.set_sign('A'), std::invalid_argument);
}

TEST(BigInt, MakePositive) {
    bigint positive("9582349582394582934859"), negative("-4123249528394572349572942345");
    positive.make_positive();
    negative.make_positive();

    EXPECT_EQ(positive.get_sign(), true);
    EXPECT_EQ(negative.get_sign(), true);
}

TEST(BigInt, MakeNegative) {
    bigint positive("9582349582394582934859"), negative("-4123249528394572349572942345");
    positive.make_negative();
    negative.make_negative();

    EXPECT_EQ(positive.get_sign(), false);
    EXPECT_EQ(negative.get_sign(), false);
}

TEST(BigInt, CountDigits) {
    EXPECT_EQ(bigint("123456789123456789123456789123456789").count_digits(), 36);
    EXPECT_EQ(bigint("-123456789123456789123456789").count_digits(), 27);
}

TEST(BigInt, Assign) {
    bigint original("-3938492834912834912834981234918234");
    bigint copy;

    copy = original;

    EXPECT_STREQ(original.get_numeric_string().c_str(), "3938492834912834912834981234918234");
    EXPECT_EQ(original.get_sign(), false);
    EXPECT_STREQ(copy.get_numeric_string().c_str(), "3938492834912834912834981234918234");
    EXPECT_EQ(copy.get_sign(), false);
}

TEST(BigInt, Equals) {
    bigint foo("2193481293412849"), bar("2193481293412849"), baz("999999999999993");
    EXPECT_TRUE(foo == bar);
    EXPECT_FALSE(foo == baz);
}

TEST(BigInt, NotEquals) {
    bigint foo("2193481293412849"), bar("2193481293412849"), baz("999999999999993");
    EXPECT_TRUE(foo != baz);
    EXPECT_FALSE(foo != bar);
}

TEST(BigInt, GreaterThan) {
    bigint big("999999999999999999999999999999999"),
         small("111111111111111111111111111111111");

    EXPECT_TRUE(big > small);
    EXPECT_FALSE(small > big);
}

TEST(BigInt, LessThan) {
    bigint big("999999999999999999999999999999999"),
         small("111111111111111111111111111111111");

    EXPECT_TRUE(small < big);
    EXPECT_FALSE(big < small);
}

TEST(BigInt, GreaterThanOrEqualTo) {
    bigint big("999999999999999999999999999999999"),
            as_big_as_first("999999999999999999999999999999999"),
            small("111111111111111111111111111111111");

    EXPECT_FALSE(small >= big);
    EXPECT_TRUE(big >= small);
    EXPECT_FALSE(small >= as_big_as_first);
    EXPECT_TRUE(big >= as_big_as_first);
    EXPECT_TRUE(as_big_as_first >= small);
    EXPECT_TRUE(as_big_as_first >= big);
}

TEST(BigInt, LessThanOrEqualTo) {
    bigint big("999999999999999999999999999999999"),
           as_big_as_first("999999999999999999999999999999999"),
           small("111111111111111111111111111111111");

    EXPECT_TRUE(small <= big);
    EXPECT_FALSE(big <= small);
    EXPECT_TRUE(small <= as_big_as_first);
    EXPECT_TRUE(big <= as_big_as_first);
    EXPECT_FALSE(as_big_as_first <= small);
    EXPECT_TRUE(as_big_as_first <= big);
}

TEST(BigInt, PrefixIncrement) {
    bigint integer("12893408123408120348120348");
    EXPECT_STREQ((++integer).get_numeric_string().c_str(), "12893408123408120348120349");
    EXPECT_STREQ(integer.get_numeric_string().c_str(),     "12893408123408120348120349");
}

TEST(BigInt, PostfixIncrement) {
    bigint integer("12893408123408120348120348");
    EXPECT_STREQ(integer++.get_numeric_string().c_str(), "12893408123408120348120348");
    EXPECT_STREQ(integer.get_numeric_string().c_str(),   "12893408123408120348120349");
}

TEST(BigInt, PrefixDecrement) {
    bigint integer("12893408123408120348120350");
    EXPECT_STREQ((--integer).get_numeric_string().c_str(), "12893408123408120348120349");
    EXPECT_STREQ(integer.get_numeric_string().c_str(),     "12893408123408120348120349");
}

TEST(BigInt, PostfixDecrement) {
    bigint integer("12893408123408120348120350");
    EXPECT_STREQ(integer--.get_numeric_string().c_str(), "12893408123408120348120350");
    EXPECT_STREQ(integer.get_numeric_string().c_str(),   "12893408123408120348120349");
}

TEST(BigInt, Addition) {
    bigint first_positive("1238491283498914"), second_positive("3408120348120350");
    bigint first_negative("-4192394192349"), second_negative("-12912349129349");
    EXPECT_STREQ((first_positive + second_positive).to_string().c_str(), "4646611631619264");
    EXPECT_STREQ((first_negative + second_negative).to_string().c_str(), "-17104743321698");
    EXPECT_STREQ((first_positive + second_negative).to_string().c_str(), "1225578934369565");
    EXPECT_STREQ((first_negative + second_positive).to_string().c_str(), "3403927953928001");
}

TEST(BigInt, Subtraction) {
    bigint first("821934819234891238491283498914"), second("12893408123408120348120350");
    EXPECT_STREQ((first - second).to_string().c_str(), "821921925826767830370935378564");
}

TEST(BigInt, Multiplication) {
    bigint first("111111111"), second("-111111111");
    EXPECT_STREQ((first * second).to_string().c_str(), "-12345678987654321");
}

TEST(BigInt, Add) {
    bigint a1("45234523452345234"), b1("2342341324234234");
    a1 += b1;
    EXPECT_STREQ(a1.to_string().c_str(), "47576864776579468");

    bigint a2("23452456523"), b2("-2346234623456");
    a2 += b2;
    EXPECT_STREQ(a2.to_string().c_str(), "-2322782166933");

    bigint a3("-56746745674567"), b3("545234523423");
    a3 += b3;
    EXPECT_STREQ(a3.to_string().c_str(), "-56201511151144");

    bigint a4("-123123123123"), b4("-104104104104");
    a4 += b4;
    EXPECT_STREQ(a4.to_string().c_str(), "-227227227227");
}

TEST(BigInt, Subtract) {
    bigint first("821934819234891238491283498914"), second("12893408123408120348120350");
    first -= second;
    EXPECT_STREQ(first.to_string().c_str(), "821921925826767830370935378564");
}

TEST(BigInt, Multiply) {
    bigint first("111111111"), second("-111111111");
    first *= second;
    EXPECT_STREQ(first.to_string().c_str(), "-12345678987654321");
}

TEST(BigInt, Negate) {
    bigint positive("9582349582394582934859"), negative("-4123249528394572349572942345");
    bigint negated_positive = -positive, negated_negative = -negative;

    EXPECT_STREQ(negated_positive.to_string().c_str(), "-9582349582394582934859");
    EXPECT_STREQ(negated_negative.to_string().c_str(), "4123249528394572349572942345");
}

TEST(BigInt, ImplicitStringCast) {
    bigint integer("-9582349582394582934859");
    std::string numeric_string = integer;

    EXPECT_STREQ(numeric_string.c_str(), "-9582349582394582934859");
}

TEST(CSV, TryParseNotExistingFile) {
    std::remove(".csv");
    EXPECT_THROW(CSV().parse_file(".csv"), std::runtime_error);
}

TEST(CSV, ParseWithoutGroupers) {
    auto table = CSV().parse("lorem,ipsum\ndolor,sit,amet\nconsectetur");

    ASSERT_EQ(table.size(), 3);
    ASSERT_EQ(table[0].size(), 2);
    ASSERT_EQ(table[1].size(), 3);
    ASSERT_EQ(table[2].size(), 1);

    EXPECT_EQ(table[0][0], "lorem"s);
    EXPECT_EQ(table[0][1], "ipsum"s);
    EXPECT_EQ(table[1][0], "dolor"s);
    EXPECT_EQ(table[1][1], "sit"s);
    EXPECT_EQ(table[1][2], "amet"s);
    EXPECT_EQ(table[2][0], "consectetur"s);
}

TEST(CSV, ParseWithGroupers) {
    auto table = CSV().parse(R"(lorem,ipsum
dolor,"sit sit sit sit",amet
consectetur)");

    ASSERT_EQ(table.size(), 3);
    ASSERT_EQ(table[0].size(), 2);
    ASSERT_EQ(table[1].size(), 3);
    ASSERT_EQ(table[2].size(), 1);

    EXPECT_EQ(table[0][0], "lorem"s);
    EXPECT_EQ(table[0][1], "ipsum"s);
    EXPECT_EQ(table[1][0], "dolor"s);
    EXPECT_EQ(table[1][1], "sit sit sit sit"s);
    EXPECT_EQ(table[1][2], "amet"s);
    EXPECT_EQ(table[2][0], "consectetur"s);
}

TEST(CSV, ParseWithGroupersAndDelimitersInThem) {
    auto table = CSV().parse(R"(lorem,ipsum
dolor,"sit sit, sit, sit",amet
consectetur)");

    ASSERT_EQ(table.size(), 3);
    ASSERT_EQ(table[0].size(), 2);
    ASSERT_EQ(table[1].size(), 3);
    ASSERT_EQ(table[2].size(), 1);

    EXPECT_EQ(table[0][0], "lorem"s);
    EXPECT_EQ(table[0][1], "ipsum"s);
    EXPECT_EQ(table[1][0], "dolor"s);
    EXPECT_EQ(table[1][1], "sit sit, sit, sit"s);
    EXPECT_EQ(table[1][2], "amet"s);
    EXPECT_EQ(table[2][0], "consectetur"s);
}

TEST(Dictionary, ConstructorEmpty) {
    Dictionary<std::string, int> empty_dictionary;
    EXPECT_EQ(empty_dictionary.size(), 0);
}

TEST(Dictionary, PuttingSignleValue) {
    Dictionary<std::string, int> dictionary;
    dictionary.put("one", 1);

    ASSERT_EQ(dictionary.size(), 1);
    ASSERT_NO_THROW(dictionary["one"]);
    EXPECT_EQ(dictionary["one"], 1);
}

TEST(Dictionary, PuttingMultipleValuesWithDifferentKeys) {
    Dictionary<std::string, int> dictionary;
    dictionary.put("one", 1);
    dictionary.put("two", 2);
    dictionary.put("three", 3);

    ASSERT_EQ(dictionary.size(), 3);
    ASSERT_NO_THROW(dictionary["one"]);
    ASSERT_NO_THROW(dictionary["two"]);
    ASSERT_NO_THROW(dictionary["three"]);
    EXPECT_EQ(dictionary["one"], 1);
    EXPECT_EQ(dictionary["two"], 2);
    EXPECT_EQ(dictionary["three"], 3);
}

TEST(Dictionary, PuttingMultipleValuesWhereSomeKeysAreTheSame) {
    Dictionary<std::string, std::string> nicknames;
    nicknames.put("Konstantin Shutov", "KostikShutov");
    nicknames.put("Anatoly Frolov", "spoke");
    nicknames.put("Anatoly Frolov", "anafro");

    ASSERT_EQ(nicknames.size(), 2);
    ASSERT_NO_THROW(nicknames["Konstantin Shutov"]);
    ASSERT_NO_THROW(nicknames["Anatoly Frolov"]);
    EXPECT_EQ(nicknames["Konstantin Shutov"], "KostikShutov"s);
    EXPECT_EQ(nicknames["Anatoly Frolov"], "anafro"s);
}

TEST(Dictionary, HasValue) {
    Dictionary<std::string, int> dictionary;
    dictionary.put("one", 1);
    dictionary.put("two", 2);
    dictionary.put("three", 3);

    EXPECT_TRUE(dictionary.has("one"));
    EXPECT_TRUE(dictionary.has("two"));
    EXPECT_TRUE(dictionary.has("three"));
    EXPECT_FALSE(dictionary.has(" three"));
    EXPECT_FALSE(dictionary.has("four"));
    EXPECT_FALSE(dictionary.has("1"));
    EXPECT_FALSE(dictionary.has("2"));
    EXPECT_FALSE(dictionary.has("3"));
}

TEST(Stack, ConstructorEmpty) {
    Stack<int> stack;

    EXPECT_FALSE(stack.has_elements());
    EXPECT_THROW(stack.pop(), std::runtime_error);
}

TEST(Stack, Manipulations) {
    Stack<int> stack;

    stack.push(5);
    EXPECT_TRUE(stack.has_elements());

    stack.push(4);
    EXPECT_TRUE(stack.has_elements());

    stack.push(3);
    EXPECT_TRUE(stack.has_elements());

    stack.push(2);
    EXPECT_TRUE(stack.has_elements());

    stack.push(1);
    EXPECT_TRUE(stack.has_elements());

    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.pop(), 1);

    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.pop(), 2);

    EXPECT_EQ(stack.top(), 3);
    EXPECT_EQ(stack.pop(), 3);

    EXPECT_EQ(stack.top(), 4);
    EXPECT_EQ(stack.pop(), 4);

    EXPECT_EQ(stack.top(), 5);
    EXPECT_EQ(stack.pop(), 5);

    EXPECT_FALSE(stack.has_elements());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}