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

	// Get the submodules as well.
	sh '''git submodule update --init'''
      }
    }
    stage('Build and Run Tests') {
      steps {
	  sh '''qmake -qt=qt5 QT+=testlib'''
	  sh '''make -j2'''
	  sh '''./Rollin --xml'''
      }
    }
  }
  post {
    always {
      xunit ( tools: [ QtTest(pattern: "*.xml") ])
    }
  }
}