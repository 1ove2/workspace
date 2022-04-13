PWD=$(pwd)
TargetName=$(basename $PWD)

if [ $# -ne 1 ]; then
    echo "Usage: $0 {x86|arm|clean}";
    exit 0;
fi

if [ $1 == "x86" ]; then
    echo "x86"
elif [ $1 == "arm" ]; then
    echo "Compile in ARM";
    source /work/tools/toolchain/fsl-imx-fb/3.14.52-1.1.1/environment-setup-cortexa9hf-vfp-neon-poky-linux-gnueabi
    unset PYTHONHOME
elif [ $1 == "clean" ]; then
    echo "make clean"
else
    echo "Unknown arg: $1";
    exit 1
fi

cd $PWD; make $1 && cp -rvf $TargetName.* /work/tftp/ && chmod 777 /work/tftp/$TargetName.* && sync
