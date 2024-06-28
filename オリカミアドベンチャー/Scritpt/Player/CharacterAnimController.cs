using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using UnityEngine.UI;

public class CharacterAnimController : MonoBehaviour
{

    //�ړI�̕�
    private GameObject targetObj;
    //�i�r���b�V���̎擾
    private NavMeshAgent agent;

    //�A�j���[�^�[
    Animator animator;
    // Start is called before the first frame update
    void Start()
    {
        //�v���C���[��Ώ�
        targetObj = GameObject.FindGameObjectWithTag("Player");

        //�ړ��ł���͈͂����肷��
        agent = this.GetComponent<NavMeshAgent>();    

        //�擾
        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        //��U�A�C�h�����O
        animator.SetBool("Running", false);

        //�ڕW�Ɍ������Ĉړ�����
        agent.SetDestination(targetObj.transform.position);
    }
}
