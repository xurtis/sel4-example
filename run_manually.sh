#!/bin/sh
set -e

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

RESULTS="$(mktemp ${TMPDIR}/health-check.XXXX.log)"
echo "Recording results to ${RESULTS}"

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
		if [ "${platform}" = "x86_64" ]; then
			images="-f $(find "${build}/images" -type f -name 'kernel-*')"
		fi
		images=" \
			${images} \
			-f $(find "${build}/images" -type f -name 'health-check-*') \
		"
		echo "Booting with ${images}"
		(mq.sh run \
			-c "Health check passed" \
			-s "${machine}" \
			${images} \
			&& (echo "MQ-PASS ${machine}" | tee -a "${RESULTS}")) \
			|| (echo "MQ-FAIL ${machine}" | tee -a "${RESULTS}")
	)
done

cat "${RESULTS}"
rm "${RESULTS}"
