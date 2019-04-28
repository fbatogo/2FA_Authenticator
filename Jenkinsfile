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
          sh '''make -j4 clean'''
          sh '''/usr/local/sonar/wrapper/build-wrapper-linux-x86-64 --out-dir bw_output make -j4 all'''
      
//	  sh '''make -j2'''
	  sh '''./Rollin --xml'''
      }
    }
    stage('SonarQube analysis') {
        
      steps {
            
        script {
          
          // requires SonarQube Scanner 2.8+
          
          def scannerHome = tool 'SonarQube Scanner';
          
          withSonarQubeEnv('SonarQube in the Cloud') {
            
            sh "${scannerHome}/bin/sonar-scanner"
          
          }
            
        }
        
      }
    
    }    
  }
  post {
    always {
      xunit ( tools: [ QtTest(pattern: "*.xml") ])
    }
  }
}