import avltree
import pytest


def test_sizeafteradd1000():
    t = avltree.Avltree()

    for i in range(1000):
        t.add(i)
    assert t.size() == 1000

def test_afteraddandremove():
    t = avltree.Avltree()
    for i in range(1000):
        t.add(i)
    for i in range(1000):
        t.remove(i)

    assert t.size() == 0





