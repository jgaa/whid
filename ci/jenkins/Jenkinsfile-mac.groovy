#!/usr/bin/env groovy

pipeline {
    agent { label 'master' }

    environment {
        WHID_VERSION = "2.0.1"
    }

    stages {
        stage('Build') {
           parallel {
                stage('Build on macOS') {
                    agent {label 'macos'}

                    environment {
                        DIST_DIR = "${WORKSPACE}/dist"
                        BUILD_DIR = "${WORKSPACE}/build"
                        SRC_DIR = "${WORKSPACE}"
                        QTDIR="/opt/Qt/5.10.0/clang_64"
                    }

                    steps {
                        echo "Building on macOS in ${WORKSPACE}"
                        checkout scm
                        sh 'pwd; ls -la;'
                        sh './scripts/package-macos.sh'
                    }

                    post {
                        success {
                            echo "Build of macOS package succeeded!"
                            archive "dist/*.dmg"
                        }
                    }
                }
            }
        }
    }
}

