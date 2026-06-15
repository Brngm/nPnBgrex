
#ifndef HANDY_TYPE_H
#define HANDY_TYPE_H

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// handy types
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
typedef long long nodeid;
// ====================================================================================
typedef std::vector<bool> vect_b;
typedef std::vector<long long> vect_l;
typedef std::vector<double> vect_d;

// typedef std::vector<long unsigned int>              vect_lu;

typedef std::vector<vect_l> vect_vect_l;
typedef std::vector<vect_d> vect_vect_d;

typedef std::set<long long> set_l;
typedef std::vector<set_l> vect_set_l;
typedef std::set<set_l> set_set_l;

typedef std::tuple<double, double> tuple_dd;
typedef std::vector<tuple_dd> vect_tuple_dd;
typedef std::vector<vect_tuple_dd> vect_vect_tuple_dd;

typedef std::tuple<long long, double, double> tuple_ldd;
typedef std::vector<tuple_ldd> vect_tuple_ldd;
typedef std::vector<vect_tuple_ldd> vect_vect_tuple_ldd;

typedef std::tuple<double, double, double> tuple_ddd;
typedef std::tuple<double, double, double, double> tuple_dddd;

typedef std::tuple<long long, long long, double> tuple_lld;
typedef std::vector<tuple_lld> vect_tuple_lld;

typedef std::tuple<long long, double> tuple_ld;
typedef std::vector<tuple_ld> vect_tuple_ld;
typedef std::vector<vect_tuple_ld> vect_vect_tuple_ld;

typedef std::tuple<long long, long long> tuple_ll;
typedef std::vector<tuple_ll> vect_tuple_ll;

typedef std::tuple<tuple_ll, vect_tuple_ll> tuple_tuple_ll_vect_tuple_ll;
typedef std::vector<tuple_tuple_ll_vect_tuple_ll>
    vect_tuple_tuple_ll_vect_tuple_ll;

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#endif