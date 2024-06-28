using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using UnityEngine.UI;

public class CharacterAnimController : MonoBehaviour
{

    //目的の物
    private GameObject targetObj;
    //ナビメッシュの取得
    private NavMeshAgent agent;

    //アニメーター
    Animator animator;
    // Start is called before the first frame update
    void Start()
    {
        //プレイヤーを対象
        targetObj = GameObject.FindGameObjectWithTag("Player");

        //移動できる範囲を限定する
        agent = this.GetComponent<NavMeshAgent>();    

        //取得
        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        //一旦アイドリング
        animator.SetBool("Running", false);

        //目標に向かって移動する
        agent.SetDestination(targetObj.transform.position);
    }
}
