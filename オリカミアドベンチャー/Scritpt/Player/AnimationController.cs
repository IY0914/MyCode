using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AnimationController : MonoBehaviour
{
    //アニメーター
    Animator animator;
    PlayerController playerController;
    public bool truefalse;

    // Start is called before the first frame update
    void Start()
    {
        //取得
        animator = GetComponent<Animator>();
        playerController = GetComponent<PlayerController>();
    }

    // Update is called once per frame
    void Update()
    {
        //操作状態なら向きを加味してアニメーション
        if(playerController.Controlller)
        {
            if (playerController.direct != 0)
            {
                truefalse = true;
            }
            else
            {
                truefalse = false;
            }

            animator.SetBool("Running", truefalse);

           
            animator.SetBool("Attack", playerController.attack);
            
        }

    }
}
