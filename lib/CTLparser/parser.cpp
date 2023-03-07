#include <CTLparser/parser.h>
#include <iostream>
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

using namespace tao::pegtl;

struct Formula;

struct Atom : identifier {};

struct True : utf8::string<'t', 'r', 'u', 'e'> {};
struct False : utf8::string<'f', 'a', 'l', 's', 'e'> {};

struct Not : utf8::one<U'¬'> {};
struct And : utf8::one<U'∧'> {};
struct Or : utf8::one<U'∨'> {};
struct Implies : utf8::one<U'⇒'> {};
struct IFF : utf8::one<U'⇔'> {};

struct RBL : utf8::one<'('> {};
struct RBR : utf8::one<')'> {};
struct SBL : utf8::one<'['> {};
struct SBR : utf8::one<']'> {};

struct A : utf8::one<'A'> {};
struct E : utf8::one<'E'> {};
struct X : utf8::one<'X'> {};
struct F : utf8::one<'F'> {};
struct G : utf8::one<'G'> {};
struct U : utf8::one<'U'> {};

struct AX : seq<A, X, Formula> {};
struct EX : seq<E, X, Formula> {};
struct AF : seq<A, F, Formula> {};
struct EF : seq<E, F, Formula> {};
struct AG : seq<A, G, Formula> {};
struct EG : seq<E, G, Formula> {};

struct PU : pad<U, space> {};
struct SFUFS : seq<SBL, Formula, PU, Formula, SBR> {};

struct AU : seq<A, SFUFS> {};
struct EU : seq<E, SFUFS> {};

struct TOP : sor<AX, EX, AF, EF, AG, EG, AU, EU> {};

struct PBOP : pad<sor<And, Or, Implies, IFF>, space> {};
struct RFOFR : seq<RBL, Formula, PBOP, Formula, RBR> {};

struct RNFR : seq<RBL, Not, Formula, RBR> {};

struct Formula : sor<TOP, RFOFR, RNFR, True, False, Atom> {};

struct Grammar : must<Formula, eof> {};

struct rearrange : parse_tree::apply<rearrange> {
    template<typename Node, typename... States>
    static void transform(std::unique_ptr<Node>& n, States&&... st) {
        if(n->children.size() == 1) {
            n = std::move(n->children[0]);
        } else if (n->children.size() == 2) {
            auto& op = n->children[0];
            op->children.push_back(std::move(n->children[1]));
            n = std::move(op);
        } else if (n->children.size() == 3) {
            auto& op = n->children[1];
            op->children.push_back(std::move(n->children[0]));
            op->children.push_back(std::move(n->children[2]));
            n = std::move(op);
        }
    }
};

template<typename Rule>
using Selector = parse_tree::selector<
    Rule,
    parse_tree::store_content::on<
        Atom
    >,
    rearrange::on<
        Formula
    >,
    parse_tree::remove_content::on<
        True,
        False,
        Not,
        And,
        Or,
        Implies,
        IFF,
        AX,
        EX,
        AF,
        EF,
        AG,
        EG,
        AU,
        EU
    >
>;

void parseFormula(const std::string& path) {
    file_input input(path);
    const auto root = parse_tree::parse<Grammar, Selector>(input);
    parse_tree::print_dot(std::cout, *root);
}
