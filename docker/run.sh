DOCKER_NAME=$1
if [[ -z `docker ps -q -f name=${DOCKER_NAME}-${USER}` ]] ; then
    docker run \
    --volume ${PWD}:/usr/local/build/src/ \
    --rm --name ${DOCKER_NAME}-${USER} -it \
    ${DOCKER_NAME}:latest bash
else
    docker exec -it ${DOCKER_NAME}-${USER} bash
fi
