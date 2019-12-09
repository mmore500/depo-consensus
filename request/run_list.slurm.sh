#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00         # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --array=0-299
#SBATCH --mem=2G                # memory required per node - amount of memory (in bytes)
#SBATCH --job-name treat=resource-even__channelsense-no__nlev-two__mute     # you can give your job a name for easier identification (same as -J)
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A_%a.out"

################################################################################
echo "Prepare Env Vars"
################################################################################

SEED_OFFSET=1000
SEED=$((SLURM_ARRAY_TASK_ID + SEED_OFFSET))
CONFUSED_COUNT=$(python3 -c "print(                                            \
  ${SLURM_ARRAY_TASK_ID} % 5                                                   \
)")
SELECTOR=$(python3 -c "print(                                                  \
 'sieve' if ${SLURM_ARRAY_TASK_ID} % 2 else 'depo'                             \
)")

TREATMENT="confused=${CONFUSED_COUNT}+selector=${SELECTOR}"

OUTPUT_DIR="/mnt/scratch/mmore500/depo-consensus/${TREATMENT}/${SEED}/"
CONFIG_DIR="/mnt/home/mmore500/depo-consensus/request/${SELECTOR}"

echo "   TREATMENT" $TREATMENT
echo "   CONFUSED_COUNT" $CONFUSED_COUNT
echo "   SELECTOR" $SELECTOR
echo "   SEED" $SEED
echo "   OUTPUT_DIR" $OUTPUT_DIR
echo "   CONFIG_DIR" $CONFIG_DIR

################################################################################
echo "Setup Work Dir"
################################################################################
rm -rf ${OUTPUT_DIR}/*  #If this is a redo, clear the dir
mkdir -p ${OUTPUT_DIR}
cp -r ${CONFIG_DIR}/* ${OUTPUT_DIR}
cd ${OUTPUT_DIR}

################################################################################
echo "Do Work"
################################################################################
module purge; module load GCC/8.2.0-2.31.1 OpenMPI/3.1.3 HDF5/1.10.4;
./depo-consensus -SEED ${SEED} -CONFUSED_COUNT ${CONFUSED_COUNT} >run.log 2>&1

###############################################################################
echo "Done"
################################################################################
