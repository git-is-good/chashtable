#include "hashtableAwesome.h"
#include <sys/types.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <unistd.h>

using namespace std;

#define SHOWDELIM printf("--------\n");

#define TIMEDECOR(func)           \
   {                              \
    time_t timeDecor_startVar = clock(); \
    printf("starting TIMEDECOR: "#func"\n"); \
    func();\
    printf("ended TIMEDECOR: "#func" time = %ld\n", clock() - timeDecor_startVar); \
   }

static const size_t standardSize = 1000000;

static void
itors(size_t n, char *buf)
{
    if ( n == 0 ){
        *buf++ = '0';
    }else{
        while ( n ){
            *buf++ = '0' + (n % 10);
            n /= 10;
        }
    }
    *buf = '\0';
    return;
}

static void
testMultiPut_cpp()
{
    char buf[64];
    const size_t testSetSize = standardSize;
#ifdef KEYTYPESTRING
    unordered_map<string, VALUETYPE> mp;
    for ( size_t i = 0; i < testSetSize; i++ ){
        itors(i, buf);
        mp[string(buf)] = (VALUETYPE)i;
    }
    for ( size_t i = 0; i < testSetSize; i++ ){
        itors(i, buf);
        mp[string(buf)] = (VALUETYPE)(i + 12);
    }
#else
    unordered_map<KEYTYPE, VALUETYPE> mp;
    for ( size_t i = 0; i < testSetSize; i++ ){
        mp[(KEYTYPE)i] = (VALUETYPE)i;
    }
    for ( size_t i = 0; i < testSetSize; i++ ){
        mp[(KEYTYPE)i] = (VALUETYPE)(i + 12);
    }
#endif
}

static void
testMultiPut_awe()
{
    char buf[64];
    const size_t testSetSize = standardSize;
    HashTable *tb = createHashTable(100);
    removeKey(tb, "hhhh");
    for ( size_t i = 0; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        putKeyValue(tb, buf, (VALUETYPE)i);
#else
        putKeyValue(tb, (KEYTYPE)i, (VALUETYPE)i);
#endif
    }
    
    for ( size_t i = 0; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        putKeyValue(tb, buf, (VALUETYPE)i);
#else
        putKeyValue(tb, (KEYTYPE)i, (VALUETYPE)i);
#endif    
    }
    destroyHashTable(tb);
}

static void
testMultiFind_cpp()
{
    char buf[64];
    const size_t testSetSize = standardSize;

#ifdef KEYTYPESTRING
    unordered_map<string, VALUETYPE> mp;
    for ( size_t i = 0; i < testSetSize; i++ ){
        itors(i, buf);
        mp[string(buf)] = (VALUETYPE)i;
    }
#else
    unordered_map<KEYTYPE, VALUETYPE> mp;
    for ( size_t i = 0; i < testSetSize; i++ ){
        mp[(KEYTYPE)i] = (VALUETYPE)i;
    }
#endif
    
    printf("cpp start finding...\n");
    time_t tm = clock();
    for ( size_t i = 0; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        assert(mp[string(buf)] == (VALUETYPE)i);
#else
        assert(mp[(KEYTYPE)i] == (VALUETYPE)i);
#endif
    }
    printf("cpp finding ended...time = %ld\n", clock() - tm);

}

static void
testMultiFind_awe()
{
    char buf[64];
    const size_t testSetSize = standardSize;

    HashTable *tb = createHashTable(100);
    for ( size_t i = 0; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        putKeyValue(tb, buf, (VALUETYPE)i);
        putKeyValue(tb, buf, (VALUETYPE)(i + 13));
#else
        putKeyValue(tb, (KEYTYPE)i, (VALUETYPE)i);
        putKeyValue(tb, (KEYTYPE)i, (VALUETYPE)(i + 13));
#endif
    }
    
    printf("awe start finding...\n");
    time_t tm = clock();
    for ( size_t i = 0; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        assert(findValueByKey(tb, buf) == (VALUETYPE)(i + 13));
#else
        assert(findValueByKey(tb, (KEYTYPE)i) == (VALUETYPE)(i + 13));
#endif
    }
    printf("awe finding ended...time = %ld\n", clock() - tm);
    destroyHashTable(tb);
}

static void
testMultiTrav_cpp()
{
    char buf[64];
    const size_t testSetSize = standardSize;
    
#ifdef KEYTYPESTRING
    unordered_map<string, VALUETYPE> mp;
    for ( size_t i = 0; i < testSetSize; i++ ){
        itors(i, buf);
        mp[string(buf)] = (VALUETYPE)i;
    }
#else
    unordered_map<KEYTYPE, VALUETYPE> mp;
    for ( size_t i = 0; i < testSetSize; i++ ){
        mp[(KEYTYPE)i] = (VALUETYPE)i;
    }
#endif
    
    printf("cpp start traversing...\n");
    time_t tm = clock();
    int count = 0;
    for ( auto iter = mp.begin(); iter != mp.end(); iter++ ){
        count++;
#ifdef KEYTYPESTRING
        itors((size_t)iter->second, buf);
        assert( strcmp(iter->first.c_str(), buf) == 0 );
#else
        assert( iter->first == (KEYTYPE)iter->second );
#endif
    }
    assert(count == testSetSize);
    printf("cpp traversing ended...time = %ld\n", clock() - tm);

}

static void
testMultiTrav_awe()
{
    char buf[64];
    const size_t testSetSize = standardSize;
    
    HashTable *tb = createHashTable(100);
    for ( size_t i = 0; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        putKeyValue(tb, buf, (VALUETYPE)i);
#else
        putKeyValue(tb, (KEYTYPE)i, (VALUETYPE)i);
#endif
    }
    
    printf("awe start traversing...\n");
    time_t tm = clock();
    HashTableIter *iter = getHashTableIter(tb);
    size_t count = 0;
    while ( hasNextElement(iter) ){
        count++;
        TableEntry *te = nextElement(iter);
#ifdef KEYTYPESTRING
        itors((size_t)te->value, buf);
        assert( strcmp(te->key, buf) == 0 );
#else
        assert( te->key == (KEYTYPE)te->value );
#endif
    }
    assert(count == testSetSize);
    destroyHashTableIter(iter);
    printf("awe traversing ended...time = %ld\n", clock() - tm);

    destroyHashTable(tb);
}

static void
testRemoval_cpp()
{
    char buf[64];
    const size_t testSetSize = standardSize;
    
#ifdef KEYTYPESTRING
    unordered_map<string, VALUETYPE> mp;
    for ( size_t i = 0; i < testSetSize; i++ ){
        itors(i, buf);
        mp[string(buf)] = (VALUETYPE)i;
    }
#else
    unordered_map<KEYTYPE, VALUETYPE> mp;
    for ( size_t i = 0; i < testSetSize; i++ ){
        mp[(KEYTYPE)i] = (VALUETYPE)i;
    }
#endif
    
    printf("cpp start removing...\n");
    time_t tm = clock();
    for ( size_t i = 0; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        mp.erase(string(buf));
#else
        mp.erase((KEYTYPE)i);
#endif
    }
    printf("cpp removing ended...time = %ld\n", clock() - tm);

}

static void
testRemoval_awe()
{
    char buf[64];
    const size_t testSetSize = standardSize;
    
    HashTable *tb = createHashTable(100);
    for ( size_t i = 0; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        putKeyValue(tb, buf, (VALUETYPE)i);
#else
        putKeyValue(tb, (KEYTYPE)i, (VALUETYPE)i);
#endif
    }
    
    printf("awe start removing...\n");
    time_t tm = clock();
    for ( size_t i = 0; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        removeKey(tb, buf);
#else
        removeKey(tb, (KEYTYPE)i);
#endif
    }
    printf("awe removing ended...time = %ld\n", clock() - tm);
    destroyHashTable(tb);
}

static void
testMultiFindAndTraversal()
{
    printf("start testMultiFindAndTraversal...\n");
    char buf[64];
    const size_t testSetSize = standardSize;
    HashTable *tb = createHashTable(100);
    for ( size_t i = 1; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        putKeyValue(tb, buf, (VALUETYPE)i);
#else
        putKeyValue(tb, (KEYTYPE)i, (VALUETYPE)i);
#endif
    }
    
    // remove some elements
    for ( size_t i = testSetSize / 3; i < testSetSize / 2; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        removeKey(tb, buf);
#else
        removeKey(tb, (KEYTYPE)i);
#endif
    }
    
    // remove nonexistent elements
    for ( size_t i = 2 * testSetSize; i < 3 * testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        removeKey(tb, buf);
#else
        removeKey(tb, (KEYTYPE)i);
#endif
    }
    for ( size_t i = 1; i < testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        VALUETYPE nd = findValueByKey(tb, buf);
#else
        VALUETYPE nd = findValueByKey(tb, (KEYTYPE)i);
#endif
        if ( i >= testSetSize / 3 && i < testSetSize / 2 ){
            assert( nd == (VALUETYPE)NONEXISTVALUE );
        }else{
            assert( nd == (VALUETYPE)i );
        }
    }
    for ( size_t i = testSetSize; i < 2 * testSetSize; i++ ){
#ifdef KEYTYPESTRING
        itors(i, buf);
        assert( findValueByKey(tb, buf) == (VALUETYPE)NONEXISTVALUE );
#else
        assert( findValueByKey(tb, (KEYTYPE)i) == (VALUETYPE)NONEXISTVALUE );
#endif
    }
    
    HashTableIter *ite = getHashTableIter(tb);
    size_t count = 0;
    while ( hasNextElement(ite) ){
        count++;
        TableEntry *nd = nextElement(ite);
        size_t val = (size_t) nd->value;
        assert(val < testSetSize / 3 || val >= testSetSize / 2);
    }
    // note we start from 1
    assert(count == testSetSize - 1 - (testSetSize/2 - testSetSize/3));
    destroyHashTableIter(ite);
    destroyHashTable(tb);
    printf("testMultiFindAndTraversal ended...\n");
}

int
main()
{
    SHOWDELIM;
    for ( int i = 0; i < 1; i++ ) TIMEDECOR(testMultiPut_cpp);
    for ( int i = 0; i < 1; i++ ) TIMEDECOR(testMultiPut_awe);
    SHOWDELIM;
  
    SHOWDELIM;
    for ( int i = 0; i < 1; i++ ) TIMEDECOR(testMultiFind_cpp);
    for ( int i = 0; i < 1; i++ ) TIMEDECOR(testMultiFind_awe);
    SHOWDELIM;
    
    SHOWDELIM;
    for ( int i = 0; i < 1; i++ ) TIMEDECOR(testMultiTrav_cpp);
    for ( int i = 0; i < 1; i++ ) TIMEDECOR(testMultiTrav_awe);
    SHOWDELIM;
    
    SHOWDELIM;
    for ( int i = 0; i < 1; i++ ) TIMEDECOR(testRemoval_cpp);
    for ( int i = 0; i < 1; i++ ) TIMEDECOR(testRemoval_awe);
    SHOWDELIM;
    
    testMultiFindAndTraversal();
    
    pause();
    return 0;
}

