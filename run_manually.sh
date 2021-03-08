#!/bin/sh

if ! command -V s4c > /dev/null; then
	alias s4c=
fi

# Get systems from machine queue
SYSTEMS=$( \
	mq.sh system-tsv \
		| tail -n +2 \
		| cut -d '	' -f 1,3 \
		| sed 's/\:[^ ]*//g' \
		| tr '	' ':' \
)

for system in ${SYSTEMS}; do
	machine=${system%:*}
	platform=${system#*:}
	if [ "${platform}" = "pc99" ]; then
		platform=x86_64
	fi

	echo "Testing ${machine} with platform ${platform}"

	build=$(realpath "build.${platform}")
	mkdir -p "${build}"

	(
		cd "${build}"
		s4c ../init-build.sh -DPLATFORM="${platform}"
		s4c ninja
		images=""
		if [ "${platform}" = "${pc99}" ]; then
			images=$(find "${build}" -type f -name 'kernel-*')
		fi
		images=\
			${images} \
			$(find "${build}" -type f -name 'mq-health-check-*')
		echo mq.sh run \
			-c "Health check passed" \
			-s "${machine}"
			${images}
	)
done
