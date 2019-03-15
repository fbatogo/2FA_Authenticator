pipeline {
  options {
    timestamps()
    buildDiscarder(logRotator(numToKeepStr: '5', artifactNumToKeepStr: '5'))
  }

  agent any
  
  stages {
    stage('Checkout Code') {
      steps {
        git 'https://github.com/fbatogo/Rollin.git'
      }
    }
    stage('Build and Run Tests') {
      steps {
	  sh '''qmake -qt=qt5 QT+=testlib'''
	  sh '''make -j2'''
	  sh '''./Rollin -o rolling-test-results.xml,xml'''
      }
    }
  }
  post {
    always {
      xunit ( tools: [ QtTest(pattern: "rolling-test-results.xml") ])
    }
  }
}