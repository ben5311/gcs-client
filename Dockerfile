FROM l.gcr.io/google/bazel:latest AS devtools

# Build environment
FROM devtools AS build

ENV SOURCE_DIR=src

COPY . $SOURCE_DIR
WORKDIR $SOURCE_DIR

#RUN bazel build ...

# App environment
FROM devtools AS app

WORKDIR /app

# Copy the program from the previously created stage and make it the entry point.
#COPY --from=build $SOURCE_DIR/bazel_out/gcs_upload /app/

CMD [ "/bin/bash" ]
