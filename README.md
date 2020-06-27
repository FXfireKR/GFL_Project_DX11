# GFL_Project 업데이트 20.06.27

 - Debug시 비 최적화로인한 Unciphing Loading을 개선
 
 - 기존 음성, 음원 과 사진이 .ab로 동일하게 Ciphing 되는것을 .acb와 .ab로 구분
 
 - M4SopMod 파일명 일부 변경
 
 - Sound, Image Load시 사용되던 내부 ifdef와 else을 제거 후 합침

 - WorldMap에 분대 배치시 사용될 Helicopter Spine Data를 추가 및 static 클래스 구현중
 
 - KSG effect 발사방향이 flip 여부에 영향을 받는것을 수정 및 signed int flipVal 추가로 방지
