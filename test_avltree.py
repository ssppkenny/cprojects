import avltree
import random
import pytest

def test_random():
    t = avltree.Avltree()
    print(t.size())
    for i in range(1000):
        n = random.randint(1,10000)
        t.add(n)
    l = t.tolist()
    for m in l:
        t.remove(m)
    assert t.size() == 0

def test_emptytolist():
    t = avltree.Avltree()
    assert t.tolist() == []

def test_sizeafteradd1000():
    t = avltree.Avltree()

    for i in range(1000):
        t.add(i)
    assert t.size() == 1000

def test_toolist():
    t = avltree.Avltree()
    for i in range(100):
        t.add(i)
    assert t.tolist() == list(range(100))

def test_afteraddandremove1():
    t = avltree.Avltree()
    N = 1000000
    for i in range(N):
        t.add(i)
    for i in range(N):
        t.remove(i)
    for i in range(N):
        t.add(i)


    assert t.size() == N 
def test_afteraddandremove():
    t = avltree.Avltree()

    for j in range(1000):
        for i in range(1000):
            t.add(i)
        for i in range(1000):
            t.remove(i)

    assert t.size() == 0


def test_contains():
    t = avltree.Avltree()
    t.add(5)
    t.add(7)
    t.add(9)
    assert t.contains(5)
    assert t.contains(7)
    assert t.contains(9)
    assert not t.contains(0)
    assert not t.contains(12)
    assert not t.contains(78)
    assert t.size() == 3







