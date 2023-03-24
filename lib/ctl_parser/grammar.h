#pragma once

#include <tao/pegtl.hpp>

using namespace tao::pegtl;

struct True : utf8::string<'t', 'r', 'u', 'e'> {};
struct False : utf8::string<'f', 'a', 'l', 's', 'e'> {};

struct Boolean : sor<True, False> {};

struct Atom : identifier {};

struct Space : utf8::one<' '> {};
struct Not : utf8::one<U'¬'> {};
struct And : pad<utf8::one<U'∧'>, Space> {};
struct Or : pad<utf8::one<U'∨'>, Space> {};
struct Imply : pad<utf8::one<U'⇒'>, Space> {};
struct IFF : pad<utf8::one<U'⇔'>, Space> {};

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

struct P0;
struct P5;

struct Bracketed : seq<RBL, P5, RBR> {};

struct AX : seq<A, X, P0> {};
struct EX : seq<E, X, P0> {};
struct AF : seq<A, F, P0> {};
struct EF : seq<E, F, P0> {};
struct AG : seq<A, G, P0> {};
struct EG : seq<E, G, P0> {};

struct PU : pad<U, Space> {};
struct SFUFS : seq<SBL, P5, PU, P5, SBR> {};

struct AU : seq<A, SFUFS> {};
struct EU : seq<E, SFUFS> {};

struct TOP : sor<AX, EX, AF, EF, AG, EG, AU, EU> {};

struct P0 : sor<TOP, Boolean, Atom, Bracketed> {};
struct P1 : seq<opt<Not>, P0> {};
struct P2 : list<P1, And> {};
struct P3 : list<P2, Or> {};
struct P4 : list<P3, Imply> {};
struct P5 : list<P4, IFF> {};

struct Grammar : must<P5, eof> {};

struct p0;
struct EQ;
struct Img;
struct FwdUntil;
struct FwdGlobal;
