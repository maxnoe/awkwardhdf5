import h5py
import pytest
from pathlib import Path
import numpy as np


@pytest.fixture
def simple_dataset_file(tmpdir):
    tmpdir = Path(tmpdir)

    path = tmpdir / 'simple_dataset.hdf5'
    with h5py.File(path, 'w') as f:
        g = f.create_group('group')
        g.create_dataset('data', data=np.random.normal(size=(10, 100)))

    return path


def test_modes(tmpdir):
    from awkwardhdf5 import File

    tmpdir = Path(tmpdir)

    path1 = tmpdir / 'test1.hdf5'
    path2 = tmpdir / 'test2.hdf5'

    assert not path1.is_file()
    assert not path2.is_file()

    File(str(path1), 'w')
    assert path1.is_file()

    File(str(path1), 'r')
    with pytest.raises(Exception):
        File(str(path2), 'r')

    File(str(path1), 'r+')
    with pytest.raises(Exception):
        File(str(path2), 'r')

    with pytest.raises(Exception):
        File(str(path1), 'x')

    File(str(path2), 'x')
    assert path2.is_file()

    File(str(path2), 'a')


def test_keys(simple_dataset_file):
    from awkwardhdf5 import File

    f = File(str(simple_dataset_file))

    assert f.keys() == ["group"]
