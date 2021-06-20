VBS selection for semi-leptonic final states on nanoAOD

Initial setup:

```bash
cmsrel CMSSW_10_6_20
git clone git@github.com:ram1123/VVjjSemileptonic-Selection.git VVjjSemileptonic
cd VVjjSemileptonic
scramv1 b -j 8
cd $CMSSW_BASE/src/VVjjSemileptonic/Selection
. getfakerates.sh
```


Run interactively: 

```bash
Selection <list of input files> <output file> <1=MC, 0=data> <era(2016 or 2017)> <DEBUG 0 or 1> <nanoaod version(7 only)>
```

```bash
Selection InputRootFileList.txt test.root 1 2018 0 8
```

Submit jobs to the fnal condor see these instructions [here](Selection/submit).
