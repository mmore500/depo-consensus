#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00         # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --array=0-149
#SBATCH --mem=8G                # memory required per node - amount of memory (in bytes)
#SBATCH --job-name depo-consensus-small     # you can give your job a name for easier identification (same as -J)
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A_%a.out"

################################################################################
echo "Prepare Env Vars"
################################################################################

SEED_OFFSET=4000
SEED=$((SLURM_ARRAY_TASK_ID + SEED_OFFSET))
CONFUSED_COUNT=$(python3 -c "print(                                            \
  ${SLURM_ARRAY_TASK_ID} % 5                                                   \
)")
CONFUSED_COUNT_2ND=$(python3 -c "print(                                        \
  (${SLURM_ARRAY_TASK_ID} % 5) * 9                                             \
)")
SELECTOR=$(python3 -c "print(                                                  \
 ['ranked', 'sieve', 'depo'][${SLURM_ARRAY_TASK_ID} % 3]                       \
)")

REPLICATE=\
"confused=${CONFUSED_COUNT}+selector=${SELECTOR}+seed=${SEED}+size=small"

OUTPUT_DIR="/mnt/scratch/mmore500/depo-consensus/${REPLICATE}/"
CONFIG_DIR="/mnt/home/mmore500/depo-consensus/request/${SELECTOR}"

echo "   REPLICATE" $REPLICATE
echo "   CONFUSED_COUNT" $CONFUSED_COUNT
echo "   CONFUSED_COUNT_2ND" $CONFUSED_COUNT_2ND
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
./depo-consensus                                                               \
  -SEED ${SEED}                                                                \
  -CONFUSED_COUNT ${CONFUSED_COUNT}                                            \
  -CONFUSED_COUNT_2ND ${CONFUSED_COUNT_2ND}                                    \
  >run.log 2>&1

###############################################################################
echo "Done"
################################################################################

qstat -f ${PBS_JOBID}
